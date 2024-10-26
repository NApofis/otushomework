#include <iostream>
#include <boost/asio.hpp>

#include "async.h"

class session : public std::enable_shared_from_this<session>
{
    boost::asio::ip::tcp::socket socket;
    char data[1024];
    const unsigned short bulk;
    void* handler;

public:
    session(boost::asio::ip::tcp::socket socket, const unsigned short bulk)
    : socket(std::move(socket))
    , bulk(bulk)
    {
        // Для каждой сессии создается свой блок. НО у всех блоков единая очередь статичекого блока.
        // Динамический блок удалится в деструкторе если он не будет закрыт до закрытия сессии
        handler = async::connect(bulk);
    }

    void start()
    {
        auto self(shared_from_this());
        socket.async_read_some(boost::asio::buffer(data, 1024),
            [this, self](boost::system::error_code ec, const std::size_t length)
            {
                if(!ec)
                {
                    async::receive(handler, data, length);
                    start();
                }
            });
    }

    ~session()
    {
        async::disconnect(handler);
    }

};


class server
{
public:
    server(boost::asio::io_context& io_context, const short port, const short bulk)
    : acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
    , bulk(bulk)
    {
        // Контрольный блок должен уничтожится когда закроется сервер и очередь команд прекратится
        control_handler = async::connect(bulk);
        do_accept();
    }

    void close() const
    {
        // Общая очередь команд жива пока жив хотя бы один Block. Поэтому пока жив сервер нужно держать один блок
        async::disconnect(control_handler);
    }

private:
    void do_accept()
    {
        acceptor.async_accept(
            [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
            {
                if (!ec)
                {
                    // Сессия не где не хранится. Поэтому перед закрытием сервера нужно закрыть все соединения (сессии).
                    // Иначе блоки не будут удалены и очередь статического блоко не будет очищена.
                    std::make_shared<session>(std::move(socket), bulk)->start();
                }

                do_accept();
            });
    }

    boost::asio::ip::tcp::acceptor acceptor;
    const unsigned short bulk;
    void* control_handler;

};

std::shared_ptr<server> server_ptr;
boost::asio::io_context io_context;

void signal_handler(const int)
{
    server_ptr->close();
    io_context.stop();
    std::cout << "Работа сервера завершена" << std::endl;
    exit(0);
}

int main(const int argc, char* argv[])
{
    try
    {
        if (argc != 3)
        {
            std::cerr << "Недостаточно данных" << std::endl;
            return 1;
        }

        std::thread server_thread([&argv]()
        {
            server_ptr = std::make_shared<server>(io_context, std::atoi(argv[1]), std::atoi(argv[2]));
            io_context.run();
        });
        signal(SIGINT, signal_handler);

        server_thread.join();
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Exception: " << ex.what() << "\n";
    }
    return 0;
}
