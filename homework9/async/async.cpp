#include "async.h"

#include <chain.h>

namespace async {


std::unique_ptr<iChain> create_chain(BlocksHandler* ptr)
{
    std::unique_ptr<iChain> chain = std::make_unique<LinkPartOfCommand>(ptr);
    chain = std::make_unique<LinkCommand>(ptr, chain);
    chain = std::make_unique<LinkBlock>(ptr, chain);
    return chain;
}

void* connect(std::size_t bulk)
{
    return BlocksHandler::create_block(bulk);
}

void receive(void* handle, const char *data, std::size_t size)
{
    BlocksHandler hBlock(handle);
    auto chain = create_chain(&hBlock);

    std::string command;
    auto add_command = [&chain, &command]()
    {
        if (!command.empty())
        {
            chain->execute(command);
        }
        command.clear();
    };

    for(int i = 0; i < size; ++i)
    {
        command += data[i];
        if(data[i] == '\n')
        {
            add_command();
        }
    }
    add_command();
}

void disconnect(void* handle)
{
    BlocksHandler::del_block(handle);
}

}
