//
// Created by ntikhonov on 21.09.24.
// Полезная утилита для быстрого поиска дублирующихся по содержимому файлов с использованием Boost
//

#include <boost/program_options.hpp>
#include <iostream>
#include "builder.h"

using namespace boost::program_options;
int main(int argc, const char * argv[])
{
    std::unique_ptr<std::string> s;
    std::unique_ptr<std::string> str((std::move(s)));


    const std::string dir = "directory";
    const std::string exc = "exclude";
    const std::string level = "level";
    const std::string size = "size";
    const std::string regex = "regex";
    const std::string block = "block_size";
    const std::string hash = "hash";


    options_description desc ("Описание опций");
    Builder build;

    desc.add_options()
        ("help,h", "this message")
        ((dir + ",d").c_str(), boost::program_options::value<std::vector<std::string>>(&build.dir())->multitoken(), "Каталог для сканирования дубликатов.")
        ((exc + ",e").c_str(), boost::program_options::value<std::vector<std::string>>(&build.exc())->multitoken(), "Каталог для исключения из сканирования. Значения могут быть разделены через ,.")
        ((level + ",l").c_str(), boost::program_options::value<unsigned short>(&build.level())->default_value(0), "Уровень сканирования. 0 - только указанный каталог без вложенных, >=1 - рекурсивный проход вглубь")
        ((size + ",m").c_str(), boost::program_options::value<unsigned int>(&build.min_size())->default_value(1), "Минимальный размер файла")
        ((regex + ",r").c_str(), boost::program_options::value<std::string>(&build.regex()), "Маска для фильтрации имен файлов")
        ((block + ",b").c_str(), boost::program_options::value<unsigned int>(&build.block_size())->default_value(4096), "Размер блока для сравнения файлов")
        ((hash + ",a").c_str(), boost::program_options::value<std::string>(&build.hash())->default_value("md5"), "Алгоритм хеширования блоков. Возможные значения md5(по умолчанию), sha1, crc.");

    parsed_options parsed = command_line_parser(argc, argv).options(desc).run();
    variables_map vm;
    store(parsed, vm);
    notify(vm);
    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
    }
    else
    {
        auto scan = build.build();
        scan.find_duplicate();
    }

    return 0;
}