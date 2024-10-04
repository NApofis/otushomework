//
// Created by ntikhonov on 20.08.24.
//
#include <iostream>
#include "chain.h"
#include "block.h"

int main(int argc, char *argv[])
{
    if(argc <= 1)
    {
        std::cerr << "Нужно указать размер блока" << std::endl;
        return 0;
    }

    std::shared_ptr<CommandBlock> block = std::make_shared<CommandBlock>(std::stoi(argv[1]));
    block->add_subscribe(std::make_unique<WriteToOutput>());
    block->add_subscribe(std::make_unique<WriteToFile>());
    std::unique_ptr<iChain> chain = std::make_unique<LinkCommand>(block);
    chain = std::make_unique<LinkBlock>(block, chain);
    chain = std::make_unique<LinkClose>(block, chain);

    std::string command;
    while (true)
    {
        std::cin >> command;
        if(std::cin.eof() || command == "EOF")
        {
            chain->execute("EOF");
            break;
        }
        chain->execute(command);
    }
}