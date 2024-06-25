#include <algorithm> // std::find_if
#include <cmath> // std::pow, std::round
#include <sstream> // std::stringstream
#include <stdexcept> // std::runtime_error

#include "utils.h"

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

// TODO: разобраться, как работает
void elapsed_time(std::string &formatted_output, size_t seconds)
{
    size_t hours = seconds / 3600;
    size_t minutes = (seconds % 3600) / 60;
    size_t sec = seconds % 60;

    std::stringstream result;

    if (hours < 10)
    {
        result << "0";
    }

    result << hours << ':';

    if (minutes < 10)
    {
        result << "0";
    }

    result << minutes << ':';

    if (sec < 10)
    {
        result << "0";
    }

    result << sec;
    formatted_output = result.str();
}