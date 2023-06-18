#include <vector>

#include "../../include/sys_info/cpu_stats.hpp"
#include "../common/system_monitor.hpp"
#include "cpu_reader/cpu_reader_linux.hpp"

void SystemMonitor::collect_cpu(SystemInfo &system_info)
{
    // TODO: инициализировать только раз
    auto reader = CpuReaderLinux(990000); // TODO: в опции

    std::vector<CpuLoad> cpu_loads_collection;
    reader.read(cpu_loads_collection);

    system_info.cpu_load_collection = std::move(cpu_loads_collection);
}

