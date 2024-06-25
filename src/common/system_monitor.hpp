#pragma once

#include "../sys_info/system_info.hpp"

/** @brief System monitoring information collector */
class SystemMonitor
{
    public:
        /** @brief Collects system information and populates @see {@SystemInfo} with it..
         * @param system_info - struct holds system monitoring information.
         */
        void collect(SystemInfo &system_info)
        {
            // TODO: распараллелить запись
            collect_load(system_info);
            collect_cpu(system_info);
        }

    private:
        /** @brief Collects information about system load average and populates @see {@SystemInfo} with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_load(SystemInfo &system_info);

        // TODO: add descr
        void collect_cpu(SystemInfo &system_info);
};