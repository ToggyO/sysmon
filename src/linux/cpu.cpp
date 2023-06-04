#include <chrono>
#include <fstream>
#include <sstream>
#include <thread>

#include "../common/system_monitor.hpp"
#include "../common/error.h"
#include "../utils/utils.h"

CpuStats read_cpu_data()
{
    std::string source_name = "/proc/stat";
    std::ifstream proc_stat(source_name);
    if (!proc_stat.is_open())
    {
        print_collection_error(source_name);
        return nullptr;
    }

    std::string line;
    getline(proc_stat, line);
    proc_stat.close();

    std::stringstream ss(line);
    std::string cpu_literal; // Skip "cpu" word
    ss >> cpu_literal;

    CpuStats cpu_stats{};
    auto *stats_p = (size_t*)&cpu_stats;
    while (ss >> *stats_p)
    {
        stats_p++;
    }

    return cpu_stats;
}

void SystemMonitor::collect_cpu(SystemInfo &system_info)
{
    auto first_measurement = read_cpu_data();

    std::this_thread::sleep_for(std::chrono::microseconds(1000));

    auto second_measurement = read_cpu_data();

    const size_t active_time = second_measurement.get_total_active() - first_measurement.get_total_active();
    const size_t idle_time = second_measurement.get_total_idle() - first_measurement.get_total_idle();
    const size_t total_time = active_time + idle_time;

    // TODO
}

