//
// Created by ntikhonov on 07.09.24.
//

#pragma once
#include <fstream>
#include <utility>

class CommandBlock;

class iWriter
{

public:
    virtual ~iWriter() = default;
    virtual void write(const CommandBlock& block) = 0;

};

class WriteToOutput final : public iWriter
{
public:
     void write(const CommandBlock&) override;
};

class WriteToFile final : public iWriter
{
public:
    void write(const CommandBlock&) override;
};