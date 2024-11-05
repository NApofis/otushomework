//
// Created by ntikhonov on 04.11.24.
//

#pragma once
#include <boost/asio.hpp>
#include <utility>

class chain;

namespace server_response
{
    const std::string OK =  "OK\n";
    const std::string ERR = "ERR";
};

struct line
{
    unsigned int id;
    std::string name;
};

class table
{
    std::string name;
    std::vector<line> lines;

public:
    explicit table(std::string  name): name(std::move(name)) {};
    std::string insert(unsigned int id, const std::string& name);
    [[nodiscard]] const decltype(lines)& get_lines() const { return lines; };
    [[nodiscard]] std::string get_name() const { return name; };
};

class memory_db
{
    std::vector<table> data;

public:
    memory_db() = default;
    table* get_or_create_table(const std::string& name);
    std::string drop_table(const std::string& name);
    [[nodiscard]] const decltype(data)& get_data() const {return data;};
    [[nodiscard]] size_t table_count() const {return data.size();};

};


class session : public std::enable_shared_from_this<session>
{
    boost::asio::ip::tcp::socket socket;
    char data[1024]{};
    std::shared_ptr<memory_db> db;
    std::shared_ptr<chain> exec;

public:
    session(boost::asio::ip::tcp::socket socket, std::shared_ptr<memory_db>& db);
    void start();
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port);

private:
    void do_accept();

    boost::asio::ip::tcp::acceptor acceptor;
    std::shared_ptr<memory_db> db;
    void* control_handler{};
};


