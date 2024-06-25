#pragma once

#include <string>
#include <vector>
#include <thread>

#include "cpu_load.hpp"
#include "disk_stats.hpp"
#include "load_average_stats.hpp"
#include "memory_stats.hpp"
#include "process.hpp"

/** @brief Struct holds system monitoring information */
struct SystemInfo
{
    /** @brief Operating system name */
    std::string os_name;
    /** @brief Operating system up time */
    size_t uptime;
    /** @brief Represents load average stats */
    LoadAverageStats load_average_stats;
    /** @brief A collection of CPU cores load */
    std::vector<CpuLoad> cpu_load_collection;
    /** @brief Represents system RAM stats */
    MemoryStats memory_stats;
    // TODO: add descr
    std::vector<DiskStats> disk_stats;
    /** @brief A collection of a system processes representations */
    std::vector<Process> processes;

    SystemInfo()
    {
        // TODO: check
        std::vector<CpuLoad> cpu_loads;
        auto cpu_cores_count = std::thread::hardware_concurrency();
        cpu_loads.reserve(cpu_cores_count);
        for (int i = 0; i < cpu_cores_count; ++i)
        {
            cpu_loads.emplace_back();
        }
        cpu_load_collection = std::move(cpu_loads);
    }
};