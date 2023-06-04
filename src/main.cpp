#include <iostream>

#include "sys_info/system_info.hpp"
#include "common/system_monitor.hpp"

int main()
{
    SystemInfo system_info{};
    SystemMonitor system_monitor{};

    system_monitor.collect(system_info);

    std::cout << system_info.load_average_stats.load_average_1 << " "
        << system_info.load_average_stats.load_average_5 << " "
        << system_info.load_average_stats.load_average_15 << std::endl;

    return 0;
}