//
// Created by ntikhonov on 20.09.24.
//

#pragma once
#include <hash.h>
#include "scaner.h"


class Builder
{
    std::vector<std::string> _directory;
    std::vector<boost::filesystem::path> scan_directory;
    std::vector<std::string> _exclude;
    unsigned short _level = 0;
    unsigned int _min_size = 0;
    unsigned int _block_size = 0;
    std::string _regex;
    boost::regex scan_regex;
    std::string _hash;
    std::unique_ptr<IHash> scan_hash;

    bool check_directory();
    bool check_regex();
    bool check_hash();

public:
    std::vector<std::string>& dir() {return _directory;}
    std::vector<std::string>& exc() {return _exclude;}
    unsigned short& level() {return _level;}
    unsigned int& min_size() {return _min_size;}
    unsigned int& block_size() {return _block_size;}
    std::string& regex() {return _regex;}
    std::string& hash() {return _hash;}

    Scan build();
};
