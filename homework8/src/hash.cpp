//
// Created by ntikhonov on 21.09.24.
//

#include "hash.h"
#include <boost/algorithm/hex.hpp>

std::string HashMd5::hash(const std::string& str)
{
    b_md5 hash;
    b_md5::digest_type digest;

    hash.process_bytes(str.c_str(), str.size());
    hash.get_digest(digest);
    constexpr size_t size = sizeof(digest);
    const auto ptr = static_cast<char*>(static_cast<void*>(digest));
    std::string result;
    boost::algorithm::hex(ptr, ptr + size, std::back_inserter(result));
    return result;
}


std::string HashSha1::hash(const std::string& str)
{
    b_sha1 hash;
    b_sha1::digest_type digest;
    hash.process_bytes(str.c_str(), str.size());
    hash.get_digest(digest);
    constexpr size_t size = sizeof(digest);
    const auto ptr = static_cast<char*>(static_cast<void*>(digest));
    std::string result;
    boost::algorithm::hex(ptr, ptr + size, std::back_inserter(result));
    return result;
}


std::string HashCrc32::hash(const std::string& str)
{
    b_crc hash;
    hash.process_bytes(str.c_str(), str.size());
    return std::to_string(hash.checksum());
}
