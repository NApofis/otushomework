//
// Created by ntikhonov on 06.09.24.
//

#include "chain.h"
#include "writer.h"

void LinkCommand::execute(const std::string& command)
{
    if(!command.empty() && !block->add_command(std::make_shared<Command>(command)) && block->check_type(Types::base))
    {
        write();
        block->clear();
    }
}

void LinkBlock::execute(const std::string& command)
{
    if(command == "{")
    {
        if(block->check_type(Types::base))
        {
            write();
            block->clear();
            block->set_type(Types::dynamic);
        }
        else if(block->check_type(Types::dynamic))
        {
            block->add_insider();
        }
    }
    else if(command == "}" && block->check_type(Types::dynamic))
    {
        if(!block->remove_insider())
        {
            write();
            block->clear();
            block->set_type(Types::base);
        }
    }
    else
    {
        return next->execute(command);
    }
}

void LinkClose::execute(const std::string& command)
{
    if(command == "EOF" && block->check_type(Types::base))
    {
        write();
    }
    else
    {
        return next->execute(command);
    }
}
