//
// Created by ntikhonov on 06.09.24.
//

#pragma once
#include <string>
#include <vector>
#include <memory>
#include "writer.h"

class Command
{
    std::string command;

public:
    explicit Command(const std::string& command) : command(command) {};
    void show(std::basic_ostream<char>& out);
};


enum class Types : uint8_t {base, dynamic};
class Block
{

    std::vector<std::shared_ptr<Command>> childrens;
    Types type = Types::base;
    std::string time_created;
    unsigned short count;
    unsigned short inside_block = 0;

public:
    explicit Block(int count) : count(count) {};

    void show(iWriter* writer);
    void clear();
    bool add_command(std::shared_ptr<Command> command);
    std::string get_time_created() const {return time_created; }

    bool check_type(const Types value) const {return type == value;}
    void set_type(const Types value) {type = value;};

    void add_insider() { inside_block++; }
    bool remove_insider();
};



