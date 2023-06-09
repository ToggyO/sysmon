#include <iomanip> 
#include <iostream> // std::setprecision


#include "sys_info/system_info.hpp"
#include "common/system_monitor.hpp"

int main()
{
    SystemInfo system_info{};
    SystemMonitor system_monitor{};

    system_monitor.collect(system_info);

    for (const auto& cpu : system_info.cpu_load_collection)
    {
        std::cout << cpu.cpu_id << ":  " << std::setprecision(2) << cpu.usage_percentage << "%" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "load average: " << system_info.load_average_stats.load_average_1 << " "
        << system_info.load_average_stats.load_average_5 << " "
        << system_info.load_average_stats.load_average_15 << std::endl;

    return 0;
}