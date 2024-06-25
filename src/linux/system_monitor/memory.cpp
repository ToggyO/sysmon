#include <fstream> // std::fstream
#include <sstream> // std::istringstream
#include <string>

#include "../../common/constants.hpp" // Constants::k_colon_delimiter
#include "../../common/error.h" // check_fs_is_open_or_throw
#include "system_monitor.hpp"

unsigned int substr_and_stoi(const std::string &value, const size_t value_start, const size_t value_end)
{
    return std::stoi(value.substr(value_start, value_end));
}

void SystemMonitor::collect_memory(SystemInfo &system_info)
{
    std::ifstream proc_meminfo(LinuxConstants::k_proc_meminfo_file_path);
    check_fs_is_open_or_throw(proc_meminfo, LinuxConstants::k_proc_meminfo_file_path);

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
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, Constants::k_colon_delimiter);
        std::getline(tokenizer, value);

        value_start = value.find_first_not_of(' ');
        value_end = value.find_first_of('k');
        if (key == LinuxConstants::k_mem_total)
        {
            total_mem = substr_and_stoi(value, value_start, value_end);
        }

        if (key == LinuxConstants::k_mem_free)
        {
            used_mem = total_mem - substr_and_stoi(value, value_start, value_end);
        }

        if (key == LinuxConstants::k_shmem)
        {
            used_mem += substr_and_stoi(value, value_start, value_end);
        }

        if (key == LinuxConstants::k_buffers  || key == LinuxConstants::k_cached || key == LinuxConstants::k_sreclaimable)
        {
            used_mem -= substr_and_stoi(value, value_start, value_end);
        }
    }

    system_info.memory_stats.total_memory = static_cast<double>(total_mem);
    system_info.memory_stats.used_memory = static_cast<double>(used_mem);
}
