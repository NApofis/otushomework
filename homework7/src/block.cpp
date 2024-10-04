//
// Created by ntikhonov on 06.09.24.
//

#include "block.h"

#include <sstream>

void Command::show(std::basic_ostream<char>& out)
{
    out << command;
}

bool CommandBlock::add_command(std::shared_ptr<Command> command)
{
    if(childrens.empty())
    {
        time_created = std::to_string(time(nullptr));
    }
    childrens.push_back(command);
    return childrens.size() != count;
}

void CommandBlock::show()
{
    for (const auto& subscriber : subscribers)
    {
        subscriber->write(*this);
    }
}

std::string CommandBlock::data_to_string() const
{
    if(childrens.empty())
    {
        return "";
    }

    std::stringstream out;
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
    return out.str();
}

bool CommandBlock::remove_insider()
{
    if(inside_block)
    {
        inside_block--;
        return true;
    }
    return false;
}

void CommandBlock::clear()
{
    childrens.clear();
}
