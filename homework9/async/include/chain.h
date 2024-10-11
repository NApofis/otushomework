//
// Created by ntikhonov on 06.09.24.
//

#pragma once

#include <memory>
#include "block.h"

class iChain {

protected:
    BlocksHandler* hBlock;
    std::unique_ptr<iChain> next = nullptr;

public:
    iChain(BlocksHandler* block, std::unique_ptr<iChain>& chain): hBlock(block), next(chain.release()) {};
    explicit iChain(BlocksHandler* block): hBlock(block) {};

    virtual void execute(const std::string& command) = 0;
    virtual ~iChain() = default;
};

class LinkCommand final : public iChain
{
public:
    LinkCommand(BlocksHandler* block, std::unique_ptr<iChain>& chain) : iChain(block, chain) {};
    explicit LinkCommand(BlocksHandler* block) : iChain(block) {};
    void execute(const std::string& command) override;
};

class LinkPartOfCommand final : public iChain
{
public:
    LinkPartOfCommand(BlocksHandler* block, std::unique_ptr<iChain>& chain) : iChain(block, chain) {};
    explicit LinkPartOfCommand(BlocksHandler* block) : iChain(block) {};
    void execute(const std::string& command) override;
};

class LinkBlock final : public iChain
{
public:

    LinkBlock(BlocksHandler* block, std::unique_ptr<iChain>& chain) : iChain(block, chain) {};
    explicit LinkBlock(BlocksHandler* block) : iChain(block) {};

    void execute(const std::string& command) override;
};
