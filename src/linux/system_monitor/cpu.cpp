#include <vector>

#include "../sys_info/cpu_stats.hpp"
#include "system_monitor.hpp"

void SystemMonitor::collect_cpu(SystemInfo &system_info)
{
    std::vector<CpuLoad> cpu_loads_collection;
    m_cpu_reader->read(cpu_loads_collection);
    system_info.cpu_load_collection = std::move(cpu_loads_collection);
}

