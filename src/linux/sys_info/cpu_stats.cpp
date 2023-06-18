#include "sys_info/cpu_stats.hpp"

size_t CpuStats::get_total_active() const
{
    return user + nice + system + irq + softirq + steal + guest + guest_nice;
}

size_t CpuStats::get_total_idle() const { return idle + iowait; }