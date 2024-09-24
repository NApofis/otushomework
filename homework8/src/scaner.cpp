//
// Created by ntikhonov on 20.09.24.
//

#include "scaner.h"

#include <iostream>
#include <boost/algorithm/string/case_conv.hpp>


std::string Scan::read_block(size_t n, File& f, std::ifstream& out)
{
    if(f.hashes.size() > n )
    {
        return f.hashes.at(n);
    }
    else if (f.full)
    {
        throw std::runtime_error("Not enough space in file");
    }

    if(!out.is_open())
    {
        out.open(f.path.get() + "/" + f.name, std::ios::binary);
        out.seekg(block_size * f.hashes.size(), std::ios::beg);
    }
    char line[block_size] = {};
    out.read(line, block_size);

    if(out.eof())
    {
        f.full = true;
        out.close();
    }
    f.hashes.push_back(hasher->hash(line));
    return f.hashes.back();
}


bool Scan::compare_files(File& file1, File& file2)
{
    std::ifstream outfile1;
    std::ifstream outfile2;
    size_t block_number = 0;
    while(true)
    {
        if(read_block(block_number, file1, outfile1) != read_block(block_number, file2, outfile2))
        {
            return false;
        }
        block_number++;
        if(file1.full && block_number == file1.hashes.size())
        {
            return file2.full && block_number == file2.hashes.size();
        }
        else if (file2.full && block_number == file2.hashes.size())
        {
            return false;
        }
    }
}


void Scan::analyze_file(boost::filesystem::path path)
{
    // Не поддерживаемые типы
    if (is_directory(path) || is_block_file(path))
    {
        return;
    }
    if(is_regular_file(path))
    {
        for(const auto& ex : exclude)
        {
            if(path.string().find(ex) != std::string::npos)
            {
                return;
            }
        }

        boost::smatch what;
        std::string str = boost::algorithm::to_lower_copy(path.filename().string());
        if(!regex.empty() && !regex_match(str, what, regex))
        {
            return;
        }

        size_t size = file_size(path);
        if(size < min_size)
        {
            return;
        }
        File file{boost::flyweights::flyweight<std::string>(path.parent_path().string()), path.filename().string(), size, {}, {}};

        for(auto& f : files)
        {
            if(f.size != file.size)
            {
                continue;
            }
            auto result = compare_files(f, file);
            if(result)
            {
                f.similarities.push_back(file);
                return;;
            }
        }
        files.push_back(file);
    }
}


void Scan::print()
{
    for(auto& f : files)
    {
        if(f.similarities.empty())
        {
            continue;
        }
        std::cout << f.path.get() + "/" + f.name << std::endl;
        for(auto& ff : f.similarities)
        {
            std::cout << ff.path.get() + "/" + ff.name << std::endl;
        }
        std::cout << std::endl;
    }
}


void Scan::check_path(boost::filesystem::path path)
{
    boost::filesystem::directory_iterator itd(path, boost::filesystem::directory_options::skip_permission_denied);
    for (boost::filesystem::directory_iterator end; itd!=end; ++itd)
    {
        analyze_file(itd->path());
    }
}

void Scan::check_path_recursive(boost::filesystem::path path)
{
    boost::filesystem::recursive_directory_iterator itd(path, boost::filesystem::directory_options::skip_permission_denied);
    for (boost::filesystem::recursive_directory_iterator end; itd!=end; ++itd)
    {
        analyze_file(itd->path());
    }
}



void Scan::find_duplicate()
{
    for(const auto& path : dirs)
    {
        if(recursive)
        {
            check_path_recursive(path);
        }
        else
        {
            check_path(path);
        }
    }
    print();
}
