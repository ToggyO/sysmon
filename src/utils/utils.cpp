#include <algorithm>
#include <sstream>
#include <stdexcept>

#include "utils.h"

// TODO: check
bool is_number(const std::string &s)
{
    return !s.empty() && std::find_if(s.begin(),
          s.end(), [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

std::string exec(std::string &command)
{
    int buffer_size = 128;
    char buffer[buffer_size];
    std::stringstream ss;

    auto pipe_reader = popen(command.c_str(), "r");
    if (!pipe_reader)
    {
        throw std::runtime_error("Failed to open pipe");
    }

    while (!feof(pipe_reader))
    {
        if (fgets(buffer, buffer_size, pipe_reader) != nullptr)
        {
            ss << buffer;
        }
    }

    pclose(pipe_reader);
    return ss.str();
}