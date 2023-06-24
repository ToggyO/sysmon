#include "system_monitor.hpp"

void SystemMonitor::collect_os_name(SystemInfo &system_info)
{
    m_common_data_reader->set_os_name(system_info.os_name);
}