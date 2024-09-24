//
// Created by ntikhonov on 20.09.24.
//
#pragma once
#include <string>
#include <fstream>
#include <boost/flyweight.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <hash.h>

struct File
{
    boost::flyweights::flyweight<std::string> path;
    std::string name;
    size_t size;
    std::vector<std::string> hashes;
    std::vector<File> similarities;
    bool full = false;
};

class Scan
{
    std::list<File> files;
    std::vector<boost::filesystem::path> dirs;
    std::vector<std::string> exclude;
    bool recursive;
    unsigned int min_size;
    unsigned int block_size;
    boost::regex regex;
    std::unique_ptr<IHash> hasher;


    void check_path(boost::filesystem::path path);
    void check_path_recursive(boost::filesystem::path path);
    void analyze_file(boost::filesystem::path path);
    bool compare_files(File& file1, File& file2);
    std::string read_block(size_t n, File& f, std::ifstream& out);
    void print();

public:
    Scan(const std::vector<boost::filesystem::path>& d, const std::vector<std::string>& e, const bool l,
         const unsigned int m, const unsigned int b, const boost::regex& r, std::unique_ptr<IHash>& h) :
    dirs(d), exclude(e), recursive(l), min_size(m), block_size(b), regex(r), hasher(h.release()) {};
    void find_duplicate();
};
