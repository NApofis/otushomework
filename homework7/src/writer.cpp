//
// Created by ntikhonov on 07.09.24.
//

#include "writer.h"

#include <fstream>

std::basic_ostream<char>& WriteToOutput::open()
{
    return std::cout;
}

void WriteToOutput::close()
{
    std::cout.flush();
}

std::basic_ostream<char>&  WriteToFile::open()
{
    file.open(fileName);
    return file;
}

void WriteToFile::close()
{
    file.close();
}
