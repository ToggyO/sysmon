#include "system_monitor.hpp"

void SystemMonitor::collect_uptime(SystemInfo &system_info)
{
    system_info.uptime = m_common_data_reader->get_system_uptime();
}