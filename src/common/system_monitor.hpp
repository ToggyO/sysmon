#pragma once

#include "../../include/sys_info/system_info.hpp"
#include "stats_reader.hpp"

/** @brief System monitoring information collector */
class SystemMonitor
{
    public:
        /** @brief Collects system information and populates SystemInfo with it..
         * @param system_info - struct holds system monitoring information.
         */
        void collect(SystemInfo &system_info)
        {
            // TODO: распараллелить запись
            collect_load(system_info);
            collect_cpu(system_info);
            collect_memory(system_info);
            collect_disks_usage(system_info);
            collect_processes_info(system_info);
        }

    private:
        void collect_os_name(SystemInfo &);
        /** @brief Collects information about system load average and populates SystemInfo with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_load(SystemInfo &);

        /** @brief Collects information about CPU usage SystemInfo with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_cpu(SystemInfo &);

        /** @brief Collects information about memory usage SystemInfo with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_memory(SystemInfo &);

        /** @brief Collects information about disks usage SystemInfo with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_disks_usage(SystemInfo &);

        /** @brief Collects information about system processes SystemInfo with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_processes_info(SystemInfo &);
};