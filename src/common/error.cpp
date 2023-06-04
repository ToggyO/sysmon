#include <iostream>

#include "error.h"

void print_collection_error(const std::string& info_source_name)
{
    std::cerr << "Can't collect system information from " << '\'' << info_source_name << '\'' << std::endl;
}