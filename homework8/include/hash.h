//
// Created by ntikhonov on 21.09.24.
//

#pragma once
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <boost/crc.hpp>

class IHash
{
public:
    virtual ~IHash() = default;
    virtual std::string hash(const std::string& str) = 0;
};

class HashMd5 final : public IHash
{
    using b_md5 = boost::uuids::detail::md5;
public:
    std::string hash(const std::string& str) override;
};

class HashSha1 final : public IHash
{
    using b_sha1 = boost::uuids::detail::sha1;
public:
    std::string hash(const std::string& str) override;
};

class HashCrc32 final : public IHash
{
    using b_crc = boost::crc_32_type;
public:
    std::string hash(const std::string& str) override;
};

