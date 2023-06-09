#include <chrono>

#include <vector>

#include "../sys_info/cpu_stats.hpp"
#include "../common/system_monitor.hpp"
#include "../utils/utils.h"
#include "cpu_reader.hpp"


void SystemMonitor::collect_cpu(SystemInfo &system_info)
{
    auto reader = CpuReaderLinux(990000);

    std::vector<CpuLoad> cpu_loads_collection;
    reader.read(cpu_loads_collection);

    system_info.cpu_load_collection = std::move(cpu_loads_collection);
}

