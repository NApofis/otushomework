//
// Created by ntikhonov on 06.09.24.
//

#pragma once
#include <shared_mutex>
#include <string>
#include <vector>
#include <list>

#include "writer.h"

class BlocksHandler
{
    struct Block
    {
        const std::size_t len_block;
        std::mutex command_mutex;

        std::atomic<unsigned int> dynamic_children_count{0};
        std::vector<std::string> commands;
        std::string* part_of_command = nullptr;

        std::string time_created;
        std::shared_timed_mutex handler_block_use_mutex;
        explicit Block(const std::size_t len_block) : len_block(len_block) {};
    };

    Block* block = nullptr;

    std::shared_lock<std::shared_timed_mutex> s_block_lock;
    static std::list<Block*> blocks;
    static std::shared_mutex blocks_lock;

    [[nodiscard]] size_t push_command(const std::string& command) const;
    void push_part_command(const std::string& command) const;
    static void write_block_command(const std::string& part_of_name,  std::vector<std::string> commands);

public:
    BlocksHandler() = default;
    explicit BlocksHandler(const void* ptr);

    void reset() const;
    void add_command(const std::string& command, bool full_command) const;

    [[nodiscard]] unsigned int add_insider() const { return block->dynamic_children_count++; }
    [[nodiscard]] unsigned int remove_insider() const { return --block->dynamic_children_count; };

    static void* create_block(size_t bulk);
    static void del_block(const void* ptr);

};



