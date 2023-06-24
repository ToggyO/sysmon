#include <algorithm>
#include <filesystem>

#include "system_monitor.hpp"

void SystemMonitor::collect_processes_info(SystemInfo &system_info)
{
    std::vector<Process> processes;

    m_process_builder->build_processes(processes);
    std::sort(processes.begin(), processes.end(), [](const Process &p1, const Process &p2)
    {
        return p2 < p1;
    });

    system_info.processes = std::move(processes);
}