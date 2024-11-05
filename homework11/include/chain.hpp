//
// Created by ntikhonov on 04.11.24.
//

#pragma once
#include <server.hpp>

class chain {

protected:
    std::shared_ptr<memory_db> db;
    std::shared_ptr<chain> next = nullptr;

public:
    chain(std::shared_ptr<memory_db> db, std::shared_ptr<chain> next): db(db), next(next) {};

    std::string execute(const std::string& command);
    [[nodiscard]] virtual std::string execute(const std::vector<std::string>& command) const = 0;
    virtual ~chain() = default;
};

class insert_chain final : public chain
{
public:
    using chain::chain;
    [[nodiscard]] std::string execute(const std::vector<std::string>& command) const override;
};

class drop_chain final : public chain
{
public:
    using chain::chain;
    [[nodiscard]] std::string execute(const std::vector<std::string>& command) const override;
};

class intersection_chain final : public chain
{
public:
    using chain::chain;
    [[nodiscard]] std::string execute(const std::vector<std::string>& command) const override;
};

class symmetric_difference_chain final : public chain
{
public:
    using chain::chain;
    [[nodiscard]] std::string execute(const std::vector<std::string>& command) const override;
};
