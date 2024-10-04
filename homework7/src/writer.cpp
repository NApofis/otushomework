//
// Created by ntikhonov on 07.09.24.
//

#include "writer.h"

#include <block.h>
#include <iostream>
#include <fstream>

void WriteToOutput::write(const CommandBlock& block)
{
    std::cout << block.data_to_string();
    std::cout.flush();
}


void WriteToFile::write(const CommandBlock& block)
{
    std::ofstream file(block.get_time_created());
    file << block.data_to_string();
    file.close();
}
