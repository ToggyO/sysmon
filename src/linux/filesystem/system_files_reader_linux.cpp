#include "system_files_reader_linux.hpp"

void create_path_from_segments(
        std::filesystem::path &result,
        const std::string &root,
        const std::string &segment2,
        const std::string &segment3)
{
    result += root;
    result += std::filesystem::path::preferred_separator;
    result += segment2;
    result += std::filesystem::path::preferred_separator;
    result += segment3;
}

void SystemFilesReaderLinux::read_proc_stat(std::stringstream &result)
{
    std::ifstream proc_stat_fs(LinuxConstants::k_proc_stat_file);
    if (!proc_stat_fs.is_open()) { return; }
//    check_fs_is_open_or_throw(proc_stat_fs, LinuxConstants::k_proc_stat_file);

    result << proc_stat_fs.rdbuf();
    proc_stat_fs.close();
}

void SystemFilesReaderLinux::read_process_stat(std::stringstream &result, const size_t &process_id)
{
    // TODO:
    //    terminate called after throwing an instance of 'std::runtime_error'
    //    what():  Cannot open: /proc/32582//stat
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(
            proc_cpu_file_path,
            LinuxConstants::k_proc_directory,
            std::to_string(process_id),
            LinuxConstants::k_proc_stat_filename);

    std::ifstream stats_fs(proc_cpu_file_path);
    if (!stats_fs.is_open()) { return; }
//    check_fs_is_open_or_throw(stats_fs, proc_cpu_file_path); TODO: check

    result << stats_fs.rdbuf();
    stats_fs.close();
}

void SystemFilesReaderLinux::read_process_cmdline(std::stringstream &result, const size_t &process_id)
{
    std::filesystem::path proc_cmd_file_path;
    create_path_from_segments(
            proc_cmd_file_path,
            LinuxConstants::k_proc_directory,
            std::to_string(process_id),
            LinuxConstants::k_cmdline_filename);

    std::ifstream cmdline_fs(proc_cmd_file_path);
    if (!cmdline_fs.is_open()) { return; }
//    check_fs_is_open_or_throw(cmdline_fs, proc_cmd_file_path);

    result << cmdline_fs.rdbuf();
    cmdline_fs.close();
}

void SystemFilesReaderLinux::read_process_etc_passwd(std::stringstream &result, const size_t &process_id)
{
    std::ifstream passwd_fs(LinuxConstants::k_pwd_path);
    if (!passwd_fs.is_open()) { return; }
//    check_fs_is_open_or_throw(passwd_fs, LinuxConstants::k_pwd_path);

    result << passwd_fs.rdbuf();
    passwd_fs.close();
}

void SystemFilesReaderLinux::read_process_status(std::stringstream &result, const size_t &process_id)
{
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(
            proc_cpu_file_path,
            LinuxConstants::k_proc_directory,
            std::to_string(process_id),
            LinuxConstants::k_proc_status_filename);

    std::ifstream status_fs(proc_cpu_file_path);
    if (!status_fs.is_open()) { return; }
//    check_fs_is_open_or_throw(status_fs, proc_cpu_file_path);

    result << status_fs.rdbuf();
    status_fs.close();
}

void SystemFilesReaderLinux::read_etc_os_release(std::stringstream &result)
{
    std::ifstream os_fs(LinuxConstants::k_os_file_path);
    if (!os_fs.is_open()) { return; }
//    check_fs_is_open_or_throw(os_fs, LinuxConstants::k_os_file_path);

    result << os_fs.rdbuf();
    os_fs.close();
}

void SystemFilesReaderLinux::read_proc_uptime(std::stringstream &result)
{
    std::ifstream uptime_fs(LinuxConstants::k_uptime_file_path);
    if (!uptime_fs.is_open()) { return; }
//    check_fs_is_open_or_throw(uptime_fs, LinuxConstants::k_uptime_file_path);

    result << uptime_fs.rdbuf();
    uptime_fs.close();
}
