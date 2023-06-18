#include <fstream> // std::fstream
#include <sstream> // std::istringstream
#include <string>

#include "../common/constants.hpp" // k_colon_delimeter
#include "../common/error.h" // check_fs_is_open_or_throw
#include "../common/system_monitor.hpp"

const static std::string k_source_name = "/proc/meminfo";
const static std::string k_mem_total = "MemTotal";
const static std::string k_mem_free = "MemFree";
const static std::string k_shmem = "Shmem";
const static std::string k_buffers = "Buffers";
const static std::string k_cached = "Cached";
const static std::string k_sreclaimable = "SReclaimable";

unsigned int substr_and_stoi(const std::string &value, const size_t value_start, const size_t value_end)
{
    return std::stoi(value.substr(value_start, value_end));
}

void SystemMonitor::collect_memory(SystemInfo &system_info)
{
    std::fstream proc_meminfo(k_source_name);
    check_fs_is_open_or_throw(proc_meminfo, k_source_name);

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
//        tokenizer = std::istringstream{line}; TODO: check
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, k_colon_delimeter);
        std::getline(tokenizer, value, k_colon_delimeter);

        value_start = value.find_first_not_of(' ');
        value_end = value.find_first_of('k');
        if (key == k_mem_total)
        {
            total_mem = substr_and_stoi(value, value_start, value_end);
        }

        if (key == k_mem_free)
        {
            used_mem = total_mem - substr_and_stoi(value, value_start, value_end);
        }

        if (key == k_shmem)
        {
            used_mem += substr_and_stoi(value, value_start, value_end);
        }

        if (key == k_buffers  || key == k_cached || key == k_sreclaimable)
        {
            used_mem -= substr_and_stoi(value, value_start, value_end);
        }
    }

    system_info.memory_stats.total_memory = static_cast<double>(total_mem);
    system_info.memory_stats.used_memory = static_cast<double>(used_mem);
}
