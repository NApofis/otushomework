//
// Created by ntikhonov on 06.09.24.
//

#include "block.h"

void Command::show(std::basic_ostream<char>& out)
{
    out << command;
}

bool Block::add_command(std::shared_ptr<Command> command)
{
    if(childrens.empty())
    {
        time_created = std::to_string(time(nullptr));
    }
    childrens.push_back(command);
    return childrens.size() != count;
}

void Block::show(iWriter* writer)
{
    if(childrens.empty())
    {
        return;
    }
    std::basic_ostream<char>& out = writer->open();
    out << "bulk: ";
    for (const auto& command : childrens)
    {
        command->show(out);
        if(command != *(childrens.end() - 1))
        {
            out << ", ";
        }
    }
    out << std::endl;
    writer->close();
}

bool Block::remove_insider()
{
    if(inside_block)
    {
        inside_block--;
        return true;
    }
    return false;
}

void Block::clear()
{
    childrens.clear();
}
