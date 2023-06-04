#pragma once

#include <string>

#include "load_average_stats.hpp"
#include "cpu_stats.hpp"

/** @brief Struct holds system monitoring information */
struct SystemInfo
{
   LoadAverageStats load_average_stats;
   // TODO: add descr
   CpuStats cpu_stats;
};