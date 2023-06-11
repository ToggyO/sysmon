#pragma once

#include "../sys_info/system_info.hpp"

/** @brief System monitoring information collector */
class SystemMonitor
{
    public:
        /** @brief Collects system information and populates @see {@link SystemInfo} with it..
         * @param system_info - struct holds system monitoring information.
         */
        void collect(SystemInfo &system_info)
        {
            // TODO: распараллелить запись
            collect_load(system_info);
            collect_cpu(system_info);
            collect_memory(system_info);
            collect_disks_usage(system_info);
        }

    private:
        /** @brief Collects information about system load average and populates @see {@link SystemInfo} with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_load(SystemInfo &system_info);

        /** @brief Collects information about CPU usage @see {@link SystemInfo} with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_cpu(SystemInfo &system_info);

        /** @brief Collects information about memory usage @see {@link SystemInfo} with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_memory(SystemInfo &system_info);

        /** @brief Collects information about disks usage @see {@link SystemInfo} with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_disks_usage(SystemInfo &system_info);
};