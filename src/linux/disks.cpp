
#include <sys/statvfs.h>

#include "../common/error.h" // print_collection_error
#include "../common/system_monitor.hpp"

void SystemMonitor::collect_disks_usage(SystemInfo &system_info)
{
    struct statvfs diskData;

    statvfs("/", &diskData);


}