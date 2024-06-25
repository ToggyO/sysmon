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

// TODO: сделать членом класса
int get_fd(const std::string& path)
{
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1)
    {
        throw std::runtime_error("Failed to open fd: " + path);
    }
    return fd;
}

SystemFilesReaderLinux::SystemFilesReaderLinux()
    : m_fds{}
{
    constexpr uint8_t fds_count = 5; // TODO: поправить!
    m_fds.reserve(fds_count);

    m_proc_stat_fd = get_fd(LinuxConstants::k_proc_stat_file);
    m_fds.push_back(m_proc_stat_fd);

    m_etc_passwd_fd = get_fd(LinuxConstants::k_pwd_path);
    m_fds.push_back(m_etc_passwd_fd);

    m_etc_os_release_fd = get_fd(LinuxConstants::k_os_file_path);
    m_fds.push_back(m_etc_os_release_fd);

    m_proc_uptime_fd = get_fd(LinuxConstants::k_uptime_file_path);
    m_fds.push_back(m_proc_uptime_fd);

    m_proc_meminfo_fd = get_fd(LinuxConstants::k_proc_meminfo_file_path);
    m_fds.push_back(m_proc_meminfo_fd);
}

void SystemFilesReaderLinux::read_proc_stat(std::stringstream &result)
{
    read_file(m_proc_stat_fd , result);
}

// TODO: вопрос по хранению fd процессов остается открытым
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

// TODO: вопрос по хранению fd процессов остается открытым
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
    read_file(m_etc_passwd_fd, result);
}

// TODO: вопрос по хранению fd процессов остается открытым
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
    read_file(m_etc_os_release_fd, result);
}

void SystemFilesReaderLinux::read_proc_uptime(std::stringstream &result)
{
    read_file(m_proc_uptime_fd, result);
}

void SystemFilesReaderLinux::read_proc_meminfo(std::stringstream& result)
{
    read_file(m_proc_meminfo_fd, result);
}

void SystemFilesReaderLinux::read_file(int fd, std::stringstream& result)
{
    lseek(fd, 0, SEEK_SET); // Set file position to 0

    constexpr size_t buffer_size = 4096;
    char buffer[buffer_size];

    size_t bytes_read;
    while ((bytes_read = read(fd, buffer, buffer_size)) > 0)
    {
        result.write(buffer, bytes_read);
    }

    if (bytes_read == -1)
    {
        throw std::runtime_error("Failed to read from file by fd");
    }
}
