//
// Created by ntikhonov on 07.09.24.
//

#pragma once
#include <fstream>
#include <iostream>
#include <utility>


class iWriter
{
public:
    virtual ~iWriter() = default;
    virtual std::basic_ostream<char>& open() =0;
    virtual void close() =0;
};

class WriteToOutput : public iWriter
{
public:
    std::basic_ostream<char>& open() override;
    void close() override;
};

class WriteToFile : public iWriter
{
    std::string fileName;
    std::ofstream file;

public:
    explicit WriteToFile(std::string fileName) : fileName(std::move(fileName)) {};
    std::basic_ostream<char>&  open() override;
    void close() override;
};