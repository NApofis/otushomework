//
// Created by ntikhonov on 06.09.24.
//

#include <iostream>
#include "block.h"

std::list<BlocksHandler::Block*> BlocksHandler::blocks = std::list<Block*>{};
std::shared_mutex BlocksHandler::blocks_lock = std::shared_mutex{};
std::vector<std::string> BlocksHandler::Block::common_commands = std::vector<std::string>{};
std::mutex BlocksHandler::Block::common_commands_mutex = std::mutex{};
std::string BlocksHandler::Block::time_created;

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


void BlocksHandler::push_command(const std::string& command) const
{
    std::vector<std::string> copy_command;
    std::string create_time;

    {
        std::lock_guard lock(Block::common_commands_mutex);
        if(Block::common_commands.empty())
        {
            Block::time_created = std::to_string(time(nullptr));
        }
        Block::common_commands.push_back(block->part_of_command + command);

        if(Block::common_commands.size() == block->len_block)
        {
            copy_command = Block::common_commands;
            create_time = Block::time_created;
            Block::time_created.clear();
            Block::common_commands.clear();
        }
    }

    if (!copy_command.empty())
    {
        write_block_command(create_time, copy_command);
    }
}


void BlocksHandler::add_command(const std::string& command, const bool full_command) const
{
    if(!full_command)
    {
        block->part_of_command += command;
        return;
    }

    if(block->dynamic_children_count == 0)
    {
        push_command(command);
    }
    else
    {
        if(block->dynamic_commands.empty())
        {
            block->dynamic_time_created = std::to_string(time(nullptr));
        }
        block->dynamic_commands.push_back(block->part_of_command + command);
    }
    if(!block->part_of_command.empty())
    {
        block->part_of_command.clear();
    }
}


void BlocksHandler::write_block_command(const std::string& part_of_name,  std::vector<std::string> commands)
{

    std::string out = "bulk: " + *commands.begin();
    for (auto command = commands.begin() + 1; command != commands.end(); ++command)
    {
        out += ", " + *command;
    }
    out += "\n";
    DCL_Actor_Writer::get_instance()->write(out, part_of_name);
}

void BlocksHandler::reset() const
{
    if (!block->dynamic_commands.empty())
    {
        write_block_command(block->dynamic_time_created, block->dynamic_commands);
        block->dynamic_commands.clear();
        block->dynamic_time_created.clear();
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
            if(blocks.size() == 1)
            {
                std::vector<std::string> copy_command;
                std::string create_time;
                {
                    std::lock_guard lock(Block::common_commands_mutex);
                    if(!Block::common_commands.empty())
                    {
                        copy_command = Block::common_commands;
                        create_time = Block::time_created;
                        Block::time_created.clear();
                        Block::common_commands.clear();
                    }
                }
                if(!copy_command.empty())
                {
                    write_block_command(create_time, copy_command);
                }
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
