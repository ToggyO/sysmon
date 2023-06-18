#include <algorithm>
#include <filesystem>

#include "../common/system_monitor.hpp"

void collect_processes(std::vector<Process> &processes)
{
    // TODO: ВЫДЕЛИТЬ ЛИНУКС КОНСТАНТЫ
    for (const auto &dir_entry : std::filesystem::directory_iterator("/proc"))
    {
        if (!dir_entry.is_directory()) { continue; }

        const auto name = dir_entry.path().filename().string();
        if (!std::all_of(name.begin(), name.end(), isdigit)) { continue; }

        processes.emplace_back((size_t)std::stoi(name));
    }
}

void SystemMonitor::collect_processes_info(SystemInfo &system_info)
{
    std::vector<Process> processes;

    collect_processes(processes);
    std::sort(processes.begin(), processes.end(), [](const Process &p1, const Process &p2)
    {
        return p2 < p1;
    });

    system_info.processes = std::move(processes);
}