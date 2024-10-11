//
// Created by ntikhonov on 06.09.24.
//

#include "block.h"

std::list<BlocksHandler::Block*> BlocksHandler::blocks = std::list<Block*>{};
std::shared_mutex BlocksHandler::blocks_lock = std::shared_mutex{};

BlocksHandler::BlocksHandler(const void* ptr)
{
    std::shared_lock l(blocks_lock);
    for(auto& b : blocks)
    {
        if(b == ptr)
        {
            block = b;
            s_block_lock = std::shared_lock(block->handler_block_use_mutex);
            return;
        }
    }
    throw std::runtime_error("Указатель не является контекстом");
}


size_t BlocksHandler::push_command(const std::string& command) const
{
    std::lock_guard lock(block->command_mutex);
    if(block->commands.empty())
    {
        block->time_created = std::to_string(time(nullptr));
    }
    if(block->part_of_command)
    {
        *block->part_of_command += command;
        block->part_of_command = nullptr;
    }
    else
    {
        block->commands.push_back(command);
    }

    return block->commands.size();
}

void BlocksHandler::push_part_command(const std::string& command) const
{
    std::lock_guard lock(block->command_mutex);

    if(block->part_of_command)
    {
        *block->part_of_command += command;
    }
    else
    {
        block->commands.push_back(command);
        block->part_of_command = &block->commands.back();
    }
}

void BlocksHandler::add_command(const std::string& command, const bool full_command) const
{
    if(!full_command)
    {
        push_part_command(command);
    }
    else if (push_command(command) >= block->len_block && block->dynamic_children_count.load() == 0)
    {
        reset();
    }
}


void BlocksHandler::write_block_command(const std::string& part_of_name,  std::vector<std::string> commands)
{

    std::string out = "bulk: ";
    for (const auto& command : commands)
    {
        out += command;
        if(command != *(commands.end() - 1))
        {
            out += ", ";
        }
    }
    out += "\n";
    DCL_Actor_Writer::get_instance()->write(out, part_of_name);
}

void BlocksHandler::reset() const
{
    std::vector<std::string> copy_command;
    std::string create_time;
    {
        std::lock_guard lock(block->command_mutex);
        copy_command = block->commands;
        create_time = block->time_created;
        block->time_created.clear();
        block->commands.clear();
    }

    if (!copy_command.empty())
    {
        write_block_command(create_time, copy_command);
    }
}

void* BlocksHandler::create_block(size_t bulk)
{
    std::unique_lock l(blocks_lock);
    auto block = new Block(bulk);
    blocks.push_back(block);
    return block;
}

void BlocksHandler::del_block(const void* ptr)
{
    std::unique_lock l(blocks_lock);
    for(auto& b : blocks)
    {
        if(b == ptr)
        {
            std::unique_lock lock(b->handler_block_use_mutex);
            if(!b->commands.empty())
            {
                write_block_command(b->time_created, b->commands);
            }
            blocks.remove(b);
            if(blocks.empty())
            {
                DCL_Actor_Writer::get_instance()->stop();
            }
            break;
        }
    }

}
