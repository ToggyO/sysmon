#include "../common/system_monitor.hpp"
#include "./common_data_reader/common_data_reader_linux.hpp"

void SystemMonitor::collect_os_name(SystemInfo &system_info)
{
    CommonDataReaderLinux::set_os_name(system_info.os_name);
}