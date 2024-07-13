//
// Created by ntikhonov on 06.07.24.
//

#include <fstream>

#include "ip_filter.h"

#include <algorithm>
#include <array>


std::tuple<bool, ip_type> read_id(const std::string& line, const char& d)
{
    const auto ip_end = line.find_first_of(d);
    if (ip_end == std::string::npos)
    {
        return {false, {}};
    }

    std::string ip = line.substr(0, ip_end);

    ip_type result;
    unsigned short ip_part_number = 0;
    auto ip_part_end = ip.find_first_of('.');
    decltype(ip_part_end) ip_part_start = 0;

    while(ip_part_end != std::string::npos)
    {
        result[ip_part_number] = static_cast<unsigned char>(std::stoi(ip.substr(ip_part_start, ip_part_end - ip_part_start)));
        ip_part_start = ip_part_end + 1;
        ip_part_end = ip.find_first_of('.', ip_part_start);
        ip_part_number++;

        if(ip_part_end == std::string::npos)
        {
            result[ip_part_number] = static_cast<unsigned char>(std::stoi(ip.substr(ip_part_start, ip.size())));
        }
    }

    return {true, result};
}

void filter_data(const std::list<ip_type>& ips, const ip_filter& _filter, std::ostream& stream)
{
    auto print = [&stream](const ip_type& ip)
    {
        for(auto it = ip.cbegin(); it < ip.end(); it++)
        {
            if(it != ip.begin())
            {
                stream << ".";
            }
            stream << std::to_string(*it);
        }
        stream << std::endl;
    };

    for (auto it = ips.rbegin(); it != ips.rend(); ++it)
    {
        auto ip_from_filter = std::get<0>(_filter);
        auto ip = *it;
        if(ip_from_filter.empty())
        {
            // Если фильтра нет тогда сразу выводим иа
            print(ip);
            continue;
        }

        // Для фильтрации any(хотя бы один байт равен переданому) и all(все байты равны) начальное состояние задается флагом
        // потому что для any условие выхода "нашли совпадение выходим" для all "НЕ нашли совпадение выходим", то есть
        // фильтер any не выполнится когда дошли до конца, а
        // фильтр all не выполнится когда не дошли до конца.
        bool found = !std::get<1>(_filter);
        for(size_t i = 0; i < ip_from_filter.size(); i++)
        {
            if (ip_from_filter[i] == ip[i] && std::get<1>(_filter))
            {
                found = true;
                break;
            }

            if (ip_from_filter[i] != ip[i] && !std::get<1>(_filter))
            {
                found = false;
                break;
            }
        }
        if(found)
        {
            print(ip);
        }
    }
}

void start_filter(std::istream& ins, std::ostream& outs)
{
    try
    {

        std::string line;
        std::list<ip_type> ips;

        while( std::getline(ins, line))
        {
            bool success;
            ip_type ip;
            std::tie(success, ip) = read_id(line, '\t');
            if(!success)
            {
                std::cerr << "Не удалось прочитать ip из строчки " << line << std::endl;
                continue;
            }
            auto place = std::find_if(ips.cbegin(), ips.cend(),
                [&ip](const ip_type& ip_from_list)->bool
                {
                    for(size_t i = 0; i < ip_from_list.size(); i++)
                    {
                        if (ip_from_list[i] != ip[i])
                        {
                            return ip_from_list[i] > ip[i];
                        }
                    }
                    return true;
                });
            ips.insert(place, ip);
        }

        filter_data(ips, {}, outs);
        filter_data(ips, {{1, }, false}, outs);
        filter_data(ips, {{46, 70}, false}, outs);
        filter_data(ips, {{46, 46, 46, 46}, true}, outs);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
