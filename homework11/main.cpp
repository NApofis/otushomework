#include <iostream>
#include <thread>

#include "server.hpp"


std::shared_ptr<server> server_ptr;
boost::asio::io_context io_context;

void signal_handler(const int)
{
    io_context.stop();
    std::cout << "Работа сервера завершена" << std::endl;
    exit(0);
}

int main(const int argc, char* argv[])
{
    try
    {
        if (argc != 2)
        {
            std::cerr << "Нужно указать порт" << std::endl;
            return 1;
        }

        std::thread server_thread([&argv]()
        {
            server_ptr = std::make_shared<server>(io_context, std::atoi(argv[1]));
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
