//
// Created by ntikhonov on 20.09.24.
//

#include "builder.h"

#include <unordered_map>
#include <hash.h>
#include <boost/algorithm/string.hpp>

bool Builder::check_directory()
{
    for (const auto& i: _directory)
    {
        auto dir = boost::filesystem::path(i);
        if(!exists(dir) || !is_directory(dir))
        {
            return false;
        }

        scan_directory.emplace_back(dir);
    }
    return true;
}

bool Builder::check_regex()
{
    try
    {
        if(_regex.empty())
        {
            return true;
        }
        const std::unordered_map<char, std::string> replacement = {
            {'.', "\\."}, {'^', "\\^"}, {'[', "\\["}, {']', "\\]"}, {'$', "\\$"},
            {'?', "."}, {'*', ".*"}
        };
        std::string new_regex;
        for(const auto& i: _regex)
        {
            if(replacement.find(i) != replacement.end())
            {
                new_regex += replacement.at(i);
            }
            else
            {
                new_regex.push_back(i);
            }
        }
        boost::algorithm::to_lower(new_regex);
        scan_regex = boost::regex(new_regex);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool Builder::check_hash()
{
    if(_hash == "md5")
    {
        scan_hash = std::make_unique<HashMd5>();
    }
    else if(_hash == "sha1")
    {
        scan_hash = std::make_unique<HashSha1>();
    }
    else if(_hash == "crc32")
    {
        scan_hash = std::make_unique<HashCrc32>();
    }
    else
    {
        return false;
    }
    return true;
}


Scan Builder::build()
{
    if(_directory.empty())
    {
        throw std::invalid_argument("Не задан каталог сканирования" );
    }
    if(!check_directory())
    {
        throw std::invalid_argument("Один из значений не является каталогом " + boost::join(_directory, ", "));
    }
    if(!check_regex())
    {
        throw std::invalid_argument("Не корректный формат маски файла " + _regex);
    }
    if(!check_hash())
    {
        throw std::invalid_argument("Не поддерживаемая функция хеширования " + _hash);
    }

    return Scan(
        scan_directory,
        _exclude,
        static_cast<bool>(_level),
        _min_size,
        _block_size,
        scan_regex,
        scan_hash
    );
}
