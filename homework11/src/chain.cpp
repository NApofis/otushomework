//
// Created by ntikhonov on 04.11.24.
//
#include <map>
#include <unordered_set>
#include <boost/algorithm/string.hpp>
#include "boost/lexical_cast.hpp"
#include "chain.hpp"


std::string chain::execute(const std::string& command)
{
    if (command.empty() || command[command.size()-1] != '\n')
    {
        return server_response::ERR + " invalid command\n";
    }
    std::vector<std::string> split_command;
    boost::split(split_command, command.substr(0, command.size()-1), boost::is_any_of(" "));
    return execute(split_command);
}


std::string insert_chain::execute(const std::vector<std::string>& command) const
{
    if(command[0] != "INSERT")
    {
        if(next)
        {
            return next->execute(command);
        }
        return server_response::ERR + " unknown command\n";
    }

    if(command.size() != 4)
    {
        return server_response::ERR + " invalid command\n";
    }

    unsigned int id;
    try
    {
        id = boost::lexical_cast<unsigned int>(command[2]);
    }
    catch (boost::bad_lexical_cast &e) {
        return server_response::ERR + " invalid argument " + command[2] + "\n";
    }

    return db->get_or_create_table(command[1])->insert(id, command[3]);
}

std::string drop_chain::execute(const std::vector<std::string>& command) const
{
    if(command[0] != "TRUNCATE")
    {
        if(next)
        {
            return next->execute(command);
        }
        return server_response::ERR + " unknown command\n";
    }
    if(command.size() != 2)
    {
        return server_response::ERR + " unknown command\n";
    }
    return db->drop_table(command[1]);
}

std::string intersection_chain::execute(const std::vector<std::string>& command) const
{
    if(command[0] != "INTERSECTION")
    {
        if(next)
        {
            return next->execute(command);
        }
        return server_response::ERR + " unknown command\n";
    }

    const auto& data = db->get_data();
    if(data.empty())
    {
        return {};
    }


    std::map<unsigned int, std::vector<std::string>> result;
    for(const auto& l : data.front().get_lines())
    {
        result.emplace(l.id, std::vector<std::string>());
        result[l.id].push_back(l.name);
    }

    for(auto it = data.cbegin() + 1; it != data.cend(); ++it)
    {
        const auto& lines = it->get_lines();
        for(auto r = result.begin(); r != result.end();)
        {

            auto f = std::find_if(lines.begin(), lines.end(),
                [check=r->first](const line& d)->bool
                {
                    return d.id == check;
                });
            if (f == lines.end())
            {
                const auto current = r;
                ++r;
                result.erase(current);
            }
            else
            {
                r->second.push_back(f->name);
                ++r;
            }
        }
    }

    std::stringstream answer;
    if(!result.empty())
    {
        for(const auto& r : result)
        {
            answer << r.first << "," << boost::join(r.second, ",") << std::endl;
        }
    }
    return answer.str() + server_response::OK;
}

std::string symmetric_difference_chain::execute(const std::vector<std::string>& command) const
{
    if(command[0] != "SYMMETRIC_DIFFERENCE")
    {
        if(next)
        {
            return next->execute(command);
        }
        return server_response::ERR + " unknown command\n";;
    }

    const auto& data = db->get_data();
    if(data.empty())
    {
        return {};
    }

    std::unordered_set<unsigned int> execute;
    std::map<unsigned int, std::vector<std::string>> result;
    for(const auto& l : data.front().get_lines())
    {
        result.emplace(l.id, std::vector<std::string>(db->table_count(), ""));
        result[l.id][0] = l.name;
    }

    for(auto it = data.cbegin() + 1; it != data.cend(); ++it)
    {
        const auto& lines = it->get_lines();
        for(const auto& line : lines)
        {
            auto f = result.find(line.id);
            if(f != result.end())
            {
                execute.emplace(line.id);
                result.erase(line.id);
            }
            else if (f == result.end() && execute.find(line.id) == execute.end())
            {
                result.emplace(line.id, std::vector<std::string>(db->table_count(), ""));
                result[line.id][it - data.cbegin()] = line.name;
            }
        }
    }

    std::stringstream answer;
    if(!result.empty())
    {
        for(const auto& r : result)
        {
            answer << r.first << "," << boost::join(r.second, ",") << std::endl;
        }
    }
    return answer.str() + server_response::OK;
}
