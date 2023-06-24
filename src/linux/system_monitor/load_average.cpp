#include "system_monitor.hpp"

void set_stats(SystemInfo &system_info, const double averages[], int averages_count)
{
    auto *stats_p = (double *)&system_info.load_average_stats;
    for (int i = 0; i < averages_count; ++i)
    {
        *stats_p = averages[i];
        stats_p++;
    }
}

void SystemMonitor::collect_load(SystemInfo &system_info)
{
    double empty = 0.00;
    int averages_count = 3;
    double averages[] { empty, empty, empty };
    if (getloadavg(averages, averages_count) < 0)
    {
        set_stats(system_info, averages, averages_count);
        return;
    }

    set_stats(system_info, averages, averages_count);
}
