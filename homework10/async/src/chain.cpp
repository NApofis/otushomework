//
// Created by ntikhonov on 06.09.24.
//

#include "chain.h"
#include "writer.h"

void LinkCommand::execute(const std::string& command)
{
    if(!command.empty() && command.back() == '\n')
    {
        hBlock->add_command(std::string(command.begin(), command.end() - 1), true);
    }
    else
    {
        return next->execute(command);
    }
}

void LinkPartOfCommand::execute(const std::string& command)
{
    if(!command.empty())
    {
        hBlock->add_command(command, false);
    }
}

void LinkBlock::execute(const std::string& command)
{
    if(command[0] == '{')
    {
        hBlock->add_insider();
    }
    else if(command[0] == '}')
    {
        if(hBlock->remove_insider() == 1)
        {
            hBlock->reset();
        }
    }
    else
    {
        return next->execute(command);
    }
}
