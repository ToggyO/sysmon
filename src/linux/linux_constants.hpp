#pragma once

#include <string>

// TODO: add descr
namespace LinuxConstants
{
    /** @brief Path to file contains information detailing how long the system has been on since its last restart */
    const static std::string k_uptime_file_path = "/proc/uptime";

    const static std::string k_os_file_path = "/etc/os-release";

    const static std::string k_pretty_name_key = "PRETTY_NAME";

    const static std::string k_proc_directory = "/proc";

    const static std::string k_proc_status_filename = "/status";

    const static std::string k_proc_stat_filename = "/stat";

    const static std::string k_cmdline_filename = "/cmdline";

    const static std::string k_pwd_path = "/etc/passwd";

    const static std::string k_uid_key = "Uid";

    const static std::string k_vm_size_key = "VmSize";

    const static ushort k_utime_index = 13;

    const static ushort k_stime_index = 14;

    const static ushort k_cutime_index = 15;

    const static ushort k_cstime_index = 16;

    const static ushort k_proc_stat_start_time_index = 21;
    /** @brief Path to file, that keeps track of a variety of different statistics about the system since it was last restarted */
    const static std::string m_source_name = "/proc/stat";
/** @brief Allows you to determine the presence of information about the loading of cpu cores when reading from the file /proc/stat */
    const static std::string m_cpu_prefix = "cpu";

    const static std::string k_proc_meminfo_file_path = "/proc/meminfo";
    const static std::string k_mem_total = "MemTotal";
    const static std::string k_mem_free = "MemFree";
    const static std::string k_shmem = "Shmem";
    const static std::string k_buffers = "Buffers";
    const static std::string k_cached = "Cached";
    const static std::string k_sreclaimable = "SReclaimable";
}