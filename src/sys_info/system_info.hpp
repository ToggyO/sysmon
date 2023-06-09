#pragma once

#include <string>
#include <vector>

#include "load_average_stats.hpp"
#include "cpu_load.hpp"

/** @brief Struct holds system monitoring information */
struct SystemInfo
{
   LoadAverageStats load_average_stats;
   // TODO: add descr
   std::vector<CpuLoad> cpu_load_collection;
};