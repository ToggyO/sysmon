#pragma once

#include "sys_info/system_info.hpp"
#include "../cpu_reader/cpu_reader_linux.hpp"
#include "../filesystem/system_files_reader.hpp"
#include "../common_data_reader/common_data_reader_linux.hpp"
#include "../process_builder/proccess_builder_linux.hpp"

/** @brief System monitoring information collector for Linux */
class SystemMonitor
{
    public:
        /** @brief Creates new instance of SystemMonitor
         * @param SystemFilesReader* Pointer to instance of Linux system file reader
         */
       explicit SystemMonitor(SystemFilesReader *files_reader) // TODO: shared_ptr
            : m_cpu_reader(new CpuReaderLinux(files_reader)),
            m_common_data_reader{new CommonDataReaderLinux(files_reader)},
            m_process_builder(new ProcessBuilderLinux(files_reader, m_common_data_reader))
        {}

        /** @brief Destroys current instance of SystemFilesReader */
        ~SystemMonitor()
        {
            delete m_common_data_reader;
            delete m_cpu_reader;
            delete m_process_builder;
        }

        /** @brief Collects system information and populates SystemInfo with it..
         * @param system_info - struct holds system monitoring information.
         */
        void collect(SystemInfo &system_info)
        {
            // TODO: распараллелить запись
            collect_os_name(system_info);
            collect_load(system_info);
            collect_cpu(system_info);
            collect_memory(system_info);
            collect_disks_usage(system_info);
            collect_processes_info(system_info);
            collect_uptime(system_info);
        }

    private:
        /** @brief Collects information about operating system name and populates SystemInfo with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_os_name(SystemInfo &);

        /** @brief Collects information about operating system up time and populates SystemInfo with it.
         * @param system_info - struct holds system monitoring information.
         */
        void collect_uptime(SystemInfo &);

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

        /** @brief Represents functionality for collecting common Linux specific data */
        CommonDataReaderLinux *const m_common_data_reader;
        /** @brief Represents functionality to read and handle CPU load */
        CpuReaderLinux *const m_cpu_reader;
        /** @bried Represents functionality for generating descriptions of Linux processes */
        ProcessBuilderLinux *const m_process_builder;
};
