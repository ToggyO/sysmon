#include <iomanip>
#include <iostream> // std::setprecision
#include <thread>

#include "../include/sys_info/system_info.hpp"
#include "common/system_monitor.hpp"
#include "./common/conversions.h" // convert_unit
#include "utils/utils.h"
#include "../libs/console/ncurses_printer.h"

void print(SystemInfo &system_info)
{
    std::cout << "[1A\r" << "OS: " << system_info.os_name << std::endl;

    std::cout << '\r' << "load average: " << std::setprecision(3)
              << system_info.load_average_stats.load_average_1 << " "
              << system_info.load_average_stats.load_average_5 << " "
              << system_info.load_average_stats.load_average_15 << std::endl;

    for (const auto& cpu : system_info.cpu_load_collection)
    {
        std::cout  << '\r' << cpu.cpu_id << ":  " << std::setprecision(2) << cpu.usage_percentage << "%" << std::endl;
    }

    double total_memory = convert_unit(static_cast<double>(system_info.memory_stats.total_memory), MEGABYTES, KILOBYTES);
    double used_memory = convert_unit(static_cast<double>(system_info.memory_stats.used_memory), MEGABYTES, KILOBYTES);
    std::cout  << '\r' << "Memory: " << std::fixed << used_memory
               << '/' << total_memory
               << " MB" << std::endl;

//        std::cout << "PID"
//            << std::setw(20) << "USER"
//            << std::setw(5) << "CPU[%]"
//            << std::setw(14) << "RAM[MB]"
//            << std::setw(12) << "TIME+"
//            << " " << "COMMAND"
//            << std::endl;
//        for (const auto &process : system_info.processes)
//        {
//            std::cout << process.pid
//                << std::setw(20) << process.user
//                << std::setw(5) << process.cpu_usage
//                << std::setw(14) << process.memory_usage
//                << std::setw(12) << process.uptime
//                << " " << process.command
//                << std::endl;
//        }

}

int main(int argc, char **argv)
{
    SystemInfo system_info{};
    SystemMonitor system_monitor{};

    // TODO: добавить сбор initial value по CPU
    print(system_info);
    while (1)
    {
        system_monitor.collect(system_info);
        print(system_info);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    //        break; // TODO: remove
    }

   return 0;
}