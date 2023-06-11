#include <iomanip> 
#include <iostream> // std::setprecision


#include "sys_info/system_info.hpp"
#include "common/system_monitor.hpp"
#include "./common/conversions.h" // convert_unit

int main()
{
    SystemInfo system_info{};
    SystemMonitor system_monitor{};

    system_monitor.collect(system_info);

    std::cout << "load average: " << system_info.load_average_stats.load_average_1 << " "
        << system_info.load_average_stats.load_average_5 << " "
        << system_info.load_average_stats.load_average_15 << std::endl;

    for (const auto& cpu : system_info.cpu_load_collection)
    {
        std::cout << cpu.cpu_id << ":  " << std::setprecision(2) << cpu.usage_percentage << "%" << std::endl;
    }

    double total_memory = convert_unit(static_cast<double>(system_info.memory_stats.total_memory), MEGABYTES, KILOBYTES);
    double used_memory = convert_unit(static_cast<double>(system_info.memory_stats.used_memory), MEGABYTES, KILOBYTES);
    std::cout << "Memory: " << std::fixed << used_memory
        << '/' << total_memory
        << " MB" << std::endl;

    return 0;
}