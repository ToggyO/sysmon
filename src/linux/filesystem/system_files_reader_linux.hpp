#pragma once

#include <filesystem>
#include <fstream> // std::ifstream

#include "../linux_constants.hpp" // m_source_name, k_proc_directory, k_proc_stat_filename, k_utime_index, k_stime_index, k_cutime_index, k_cstime_index, k_cmdline_filename, k_pwd_path, k_proc_status_filename, k_proc_stat_start_time_index, k_vm_size_key, k_uid_key
#include "system_files_reader.hpp"
#include "../../common/error.h" // check_fs_is_open_or_throw

/** @bried Interface implementation for reading Linux system files */
class SystemFilesReaderLinux : public SystemFilesReader
{
    public:
        /** @brief Read data from '/proc/stat'
         * @override
         * @param std::stringstream Reference to result variable
        */
        void read_proc_stat(std::stringstream &) override;

        /** @brief Read data from '/proc/<pid>/stat'
         * @override
         * @param std::stringstream Reference to result variable
         * @param size_t Process identifier @constant
        */
        void read_process_stat(std::stringstream &, const size_t &) override;

        /** @brief Read data from '/proc/<pid>/cmdline'
         * @override
         * @param std::stringstream Rereferense to result variable
         * @param size_t Process identifier @constant
        */
        void read_process_cmdline(std::stringstream &, const size_t &) override;

        /** @brief Read data from '/etc/passwd'
         * @override
         * @param std::stringstream Reference to result variable
         * @param size_t Process identifier @constant
        */
        void read_process_etc_passwd(std::stringstream &, const size_t &) override;

        /** @brief Read data from '/proc/<pid>/status'
         * @override
         * @param std::stringstream Reference to result variable
         * @param size_t Process identifier @constant
        */
        void read_process_status(std::stringstream &, const size_t &) override;

        /** @brief Read data from '/etc/os-release'
         * @override
         * @param std::stringstream Reference to result variable
        */
        void read_etc_os_release(std::stringstream &) override;

        /** @brief Read data from '/proc/uptime'
         * @override
         * @param std::stringstream Reference to result variable
        */
        void read_proc_uptime(std::stringstream &) override;
};