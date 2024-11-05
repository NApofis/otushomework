//
// Created by ntikhonov on 04.11.24.
//

#include "server.hpp"

#include <chain.hpp>
#include <iostream>
#include <boost/asio.hpp>

table* memory_db::get_or_create_table(const std::string& name)
{
    for(auto& table : data)
    {
        if(table.get_name() == name)
        {
            return &table;
        }
    }
    return &(data.emplace_back(name));
}

std::string table::insert(unsigned int id, const std::string& name)
{
    for(auto it = lines.begin(); it != lines.end(); ++it)
    {
        if(it->id == id)
        {
            return server_response::ERR + " duplicate " + std::to_string(it->id) + "\n";
        }
        if(it->id > id)
        {
            lines.insert(it, {id, name});
            return server_response::OK;
        }
    }
    lines.push_back({id, name});
    return server_response::OK;
}

std::string memory_db::drop_table(const std::string& name)
{
    for(auto i = data.cbegin(); i < data.cend(); ++i)
    {
        if(i->get_name() == name)
        {
            data.erase(i);
            return server_response::OK;
        }
    }
    return server_response::ERR + " unknown table " + name + "\n";
}

session::session(boost::asio::ip::tcp::socket socket, std::shared_ptr<memory_db>& db): socket(std::move(socket)) , db(db)
{
    std::shared_ptr<chain> obj = std::make_shared<symmetric_difference_chain>(db, nullptr);
    obj = std::make_shared<intersection_chain>(db, obj);
    obj = std::make_shared<drop_chain>(db, obj);
    exec = std::make_shared<insert_chain>(db, obj);
}


void session::start()
{
    auto self(shared_from_this());
    socket.async_read_some(boost::asio::buffer(data, 1024),
        [this, self](boost::system::error_code ec, const std::size_t length)
        {
            if(!ec)
            {
                std::string answer = exec->execute(std::string(data, length));
                boost::asio::async_write(socket, boost::asio::buffer(answer.c_str(), answer.size()),
                    [this, self](const boost::system::error_code& error, std::size_t)
                    {
                        if(!error)
                        {
                            start();
                        }
                    });
            }
        });
}
server::server(boost::asio::io_context& io_context, const short port) :
    acceptor(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
{
    do_accept();
    db = std::make_shared<memory_db>();
}

void server::do_accept()
{
    acceptor.async_accept(
        [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
        {
            if (!ec)
            {
                // Сессия не где не хранится. Поэтому перед закрытием сервера нужно закрыть все соединения (сессии).
                // Иначе блоки не будут удалены и очередь статического блоко не будет очищена.
                std::make_shared<session>(std::move(socket), db)->start();
            }

            do_accept();
        });
}

