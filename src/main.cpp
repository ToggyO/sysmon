#include <iomanip>
#include <iostream> // std::setprecision
#include <thread>

#include "common/system_monitor.hpp"
#include "./common/conversions.h" // convert_unit
#include "utils/utils.h"
#include "system_monitor_factory.hpp"

#if defined(_WIN32) || defined(_WIN64)
#endif

#if defined(__APPLE__) && defined(__MACH__)
#endif

#if defined(linux) || defined(__linux__) || defined(__linux)
#include "linux/system_monitor/system_monitor.hpp"
#endif

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

volatile sig_atomic_t stop;
void sig_handler(int signum) { stop = 1; }

// TODO: НАТЫКАТЬ САНИТАЙЗЕРЫ
int main(int argc, char **argv)
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);

    SystemInfo system_info{};
    SystemMonitorFactory factory{};
    const auto system_monitor = factory.create();

    // TODO: добавить сбор initial value по CPU
    print(system_info);
    while (!stop)
    {
        system_monitor->collect(system_info);
        print(system_info);
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
//        break; // TODO: remove
    }

    // ВОПРОС: вероятно, очистка здесь лишняя, т.к. домен приложение в будет выгружен в любом случае после завершенния цикла
    // Очистка вызывает задержку в завершении программы
    factory.clear(system_monitor);
    return 0;
}