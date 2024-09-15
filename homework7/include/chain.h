//
// Created by ntikhonov on 06.09.24.
//

#pragma once

#include <memory>
#include <utility>
#include "block.h"

class iChain {

protected:
    std::shared_ptr<Block> block;
    std::unique_ptr<iChain> next = nullptr;

    inline void write() const
    {
        auto console = WriteToOutput();
        block->show(&console);
        auto file = WriteToFile(block->get_time_created());
        block->show(&file);
    }

public:
    iChain(std::shared_ptr<Block> block, std::unique_ptr<iChain>& chain): block(block), next(chain.release()) {};
    explicit iChain(std::shared_ptr<Block> block): block(block) {};

    virtual void execute(const std::string& command) = 0;
    virtual ~iChain() = default;
};

class LinkCommand : public iChain
{
public:
    LinkCommand(std::shared_ptr<Block> block, std::unique_ptr<iChain>& chain) : iChain(block, chain) {};
    explicit LinkCommand(std::shared_ptr<Block> block) : iChain(block) {};
    void execute(const std::string& command) override;
};


class LinkBlock : public iChain
{
public:

    LinkBlock(std::shared_ptr<Block> block, std::unique_ptr<iChain>& chain) : iChain(block, chain) {};
    explicit LinkBlock(std::shared_ptr<Block> block) : iChain(block) {};

    void execute(const std::string& command) override;
};

class LinkClose : public iChain
{
public:
    LinkClose(std::shared_ptr<Block> block, std::unique_ptr<iChain>& chain) : iChain(block, chain) {};
    explicit LinkClose(std::shared_ptr<Block> block) : iChain(block) {};

    void execute(const std::string& command) override;
};
