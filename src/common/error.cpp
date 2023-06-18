#include <iostream>

#include "error.h"

void print_collection_error(const std::string& info_source_name)
{
    std::cerr << "Can't collect system information from " << '\'' << info_source_name << '\'' << std::endl;
}

void check_fs_is_open_or_throw(const std::fstream &fs, const std::string &fs_source_name)
{
    if (!fs.is_open())
    {
        print_collection_error(fs_source_name);
        throw std::runtime_error("Cannot open: " + fs_source_name);
    }
}