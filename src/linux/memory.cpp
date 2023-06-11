#include <fstream> // std::ifstream 
#include <sstream> // std::istringstream
#include <string>
#include <iostream>

#include "../common/error.h" // print_collection_error
#include "../common/system_monitor.hpp"


const static char m_delimeter = ':';
const static std::string m_source_name = "/proc/meminfo";
const static std::string m_mem_total = "MemTotal";
const static std::string m_mem_free = "MemFree";
const static std::string m_shmem = "Shmem";
const static std::string m_buffers = "Buffers";
const static std::string m_cached = "Cached";
const static std::string m_sreclaimable = "SReclaimable";

unsigned int substr_and_stoi(const std::string &value, const size_t value_start, const size_t value_end)
{
    return std::stoi(value.substr(value_start, value_end));
}

void SystemMonitor::collect_memory(SystemInfo &system_info)
{
    std::ifstream proc_meminfo(m_source_name);
        if (!proc_meminfo.is_open())
    {
        print_collection_error(m_source_name);
        // TODO: duplicate
        throw std::runtime_error("Cannot open: " + m_source_name);
    }


    std::string line;
    std::string key;
    std::string value;
    std::istringstream tokenizer;
    size_t value_start = 0;
    size_t value_end = 0;

    unsigned int total_mem = 0;
    unsigned int used_mem = 0;

    while (getline(proc_meminfo, line))
    {
        tokenizer = std::istringstream{line};

        std::getline(tokenizer, key, m_delimeter);
        std::getline(tokenizer, value, m_delimeter);

        value_start = value.find_first_not_of(" ");
        value_end = value.find_first_of('k');
        if (key == m_mem_total)
        {
            total_mem = substr_and_stoi(value, value_start, value_end);
        }

        if (key == m_mem_free)
        {
            used_mem = total_mem - substr_and_stoi(value, value_start, value_end);
        }

        if (key == m_shmem)
        {
            used_mem += substr_and_stoi(value, value_start, value_end);
        }

        if (key == m_buffers  || key == m_cached || key == m_sreclaimable)
        {
            used_mem -= substr_and_stoi(value, value_start, value_end);
        }
    }

    system_info.memory_stats.total_memory = static_cast<double>(total_mem);
    system_info.memory_stats.used_memory = static_cast<double>(used_mem);
}
