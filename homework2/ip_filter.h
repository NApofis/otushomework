#pragma once

#include <iostream>
#include <tuple>
#include <vector>
#include <list>

using ip_type = std::array<unsigned char, 4>;
using ip_filter = std::tuple<std::vector<unsigned char>, bool>; // filter_ip, {any: true, all: false}

void start_filter(std::istream& ins, std::ostream& outs);
void filter_data(const std::list<ip_type>& ips, const ip_filter& _filter, std::ostream& stream);
std::tuple<bool, ip_type> read_id(const std::string& line, const char& d);

