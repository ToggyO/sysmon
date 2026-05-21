#include "system_files_reader_linux.hpp"

// TODO: файловые дексрипторы надо не забывать закрывать ЛОООООЛ
SystemFilesReaderLinux::SystemFilesReaderLinux(std::shared_ptr<IFileDescriptorsCache> fd_cache_ptr)
    : m_fd_cache_ptr{std::move(fd_cache_ptr)}
{
    m_fd_cache_ptr->save_by_file_path(LinuxConstants::k_proc_stat_file, create_fd(LinuxConstants::k_proc_stat_file));
    m_fd_cache_ptr->save_by_file_path(LinuxConstants::k_pwd_path, create_fd(LinuxConstants::k_pwd_path));
    m_fd_cache_ptr->save_by_file_path(LinuxConstants::k_os_file_path, create_fd(LinuxConstants::k_os_file_path));
    m_fd_cache_ptr->save_by_file_path(LinuxConstants::k_uptime_file_path,
                                      create_fd(LinuxConstants::k_uptime_file_path));
    m_fd_cache_ptr->save_by_file_path(LinuxConstants::k_proc_meminfo_file_path,
                                      create_fd(LinuxConstants::k_proc_meminfo_file_path));
}

void SystemFilesReaderLinux::read_proc_stat(std::stringstream& result)
{
    read_file(get_fd_or_throw(LinuxConstants::k_proc_stat_file), result);
}

// TODO: вопрос по хранению fd процессов остается открытым
void SystemFilesReaderLinux::read_process_stat(std::stringstream& result, const size_t& process_id)
{
    // TODO:
    //    terminate called after throwing an instance of 'std::runtime_error'
    //    what():  Cannot open: /proc/32582/stat
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(proc_cpu_file_path, LinuxConstants::k_proc_directory, std::to_string(process_id),
                              LinuxConstants::k_proc_stat_filename);

    std::ifstream stats_fs(proc_cpu_file_path);
    if (!stats_fs.is_open())
    {
        return;
    } // TODO: как будто хуита
      //    check_fs_is_open_or_throw(stats_fs, proc_cpu_file_path); TODO: check

    result << stats_fs.rdbuf();
    stats_fs.close();

    // auto fd_opt = m_fd_cache_ptr->get_by_pid(process_id);
    // if (!fd_opt.has_value())
    // {
    //     fd_opt = ProcessFdsSet{};
    // }

    // if (!fd_opt->proc_stat_fd.has_value())
    // {
    //     std::filesystem::path proc_stat_file_path;
    //     create_path_from_segments(
    //             proc_stat_file_path,
    //             LinuxConstants::k_proc_directory,
    //             std::to_string(process_id),
    //             LinuxConstants::k_proc_stat_filename);

    //     int fd = create_fd(proc_stat_file_path);
    //     if (fd == -1)
    //     {
    //         return;
    //     }

    //     fd_opt->proc_stat_fd = {fd};
    //     m_fd_cache_ptr->save_by_pid(process_id, fd_opt.value());
    // }

    // try
    // {
    //     read_file(fd_opt->proc_stat_fd.value(), result);
    // }
    // catch (...)
    // {
    //     return;
    // }

    //    std::cout << result.str() << std::endl;
}

// TODO: вопрос по хранению fd процессов остается открытым
void SystemFilesReaderLinux::read_process_cmdline(std::stringstream& result, const size_t& process_id)
{
    std::filesystem::path proc_cmd_file_path;
    create_path_from_segments(proc_cmd_file_path, LinuxConstants::k_proc_directory, std::to_string(process_id),
                              LinuxConstants::k_cmdline_filename);

    std::ifstream cmdline_fs(proc_cmd_file_path);
    if (!cmdline_fs.is_open())
    {
        return;
    } // TODO: как будто хуита
      //    check_fs_is_open_or_throw(cmdline_fs, proc_cmd_file_path);

    result << cmdline_fs.rdbuf();
    cmdline_fs.close();

    // auto fd_opt = m_fd_cache_ptr->get_by_pid(process_id);
    // if (!fd_opt.has_value())
    // {
    //     fd_opt = ProcessFdsSet{};
    // }

    // if (!fd_opt->proc_cmd_fd.has_value())
    // {
    //     std::filesystem::path proc_cmd_file_path;
    //     create_path_from_segments(
    //             proc_cmd_file_path,
    //             LinuxConstants::k_proc_directory,
    //             std::to_string(process_id),
    //             LinuxConstants::k_cmdline_filename);

    //     int fd = create_fd(proc_cmd_file_path);
    //     if (fd == -1)
    //     {
    //         std::cerr << "Error opening file: " << strerror(errno) << std::endl;
    //         return;
    //     }

    //     fd_opt->proc_cmd_fd = {fd};
    //     m_fd_cache_ptr->save_by_pid(process_id, fd_opt.value());
    // }

    // try
    // {
    //     read_file(fd_opt->proc_cmd_fd.value(), result);
    // }
    // catch (...)
    // {
    //     return;
    // }

    //    std::cout << result.str() << std::endl; TODO: remove
}

void SystemFilesReaderLinux::read_process_etc_passwd(std::stringstream& result, const size_t& process_id)
{
    read_file(get_fd_or_throw(LinuxConstants::k_pwd_path), result);
}

// TODO: вопрос по хранению fd процессов остается открытым
void SystemFilesReaderLinux::read_process_status(std::stringstream& result, const size_t& process_id)
{
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(proc_cpu_file_path, LinuxConstants::k_proc_directory, std::to_string(process_id),
                              LinuxConstants::k_proc_status_filename);

    std::ifstream status_fs(proc_cpu_file_path);
    if (!status_fs.is_open())
    {
        return;
    } // TODO: как будто хуита

    result << status_fs.rdbuf();
    //    std::cout << result.str() << std::endl;
    status_fs.close();
    //    result.str("");

    // auto fd_opt = m_fd_cache_ptr->get_by_pid(process_id);
    // if (!fd_opt.has_value())
    // {
    //     fd_opt = ProcessFdsSet{};
    // }

    // if (!fd_opt->proc_status_fd.has_value())
    // {
    //     std::filesystem::path proc_status_file_path;
    //     create_path_from_segments(
    //             proc_status_file_path,
    //             LinuxConstants::k_proc_directory,
    //             std::to_string(process_id),
    //             LinuxConstants::k_proc_status_filename);

    //     int fd = create_fd(proc_status_file_path);
    //     if (fd == -1)
    //     {
    //         return;
    //     }

    //     fd_opt->proc_status_fd = fd;
    //     m_fd_cache_ptr->save_by_pid(process_id, fd_opt.value());
    // }

    // try
    // {
    //     read_file(fd_opt->proc_status_fd.value(), result);
    // }
    // catch (...)
    // {
    //     return;
    // }
}

void SystemFilesReaderLinux::read_etc_os_release(std::stringstream& result)
{
    read_file(get_fd_or_throw(LinuxConstants::k_os_file_path), result);
}

void SystemFilesReaderLinux::read_proc_uptime(std::stringstream& result)
{
    read_file(get_fd_or_throw(LinuxConstants::k_uptime_file_path), result);
}

void SystemFilesReaderLinux::read_proc_meminfo(std::stringstream& result)
{
    read_file(get_fd_or_throw(LinuxConstants::k_proc_meminfo_file_path), result);
}

void SystemFilesReaderLinux::read_file(int fd, std::stringstream& result)
{
    lseek(fd, 0, SEEK_SET); // Set file position to 0

    constexpr size_t buffer_size = 4096;
    char buffer[buffer_size];

    std::streamsize bytes_read;
    while ((bytes_read = read(fd, buffer, buffer_size)) > 0)
    {
        result.write(buffer, bytes_read);
    }

    if (bytes_read == -1)
    {
        throw std::runtime_error("Failed to read from file by fd");
    }
}

void SystemFilesReaderLinux::create_path_from_segments(std::filesystem::path& result, const std::string& root,
                                                       const std::string& segment2, const std::string& segment3)
{
    result += root;
    result += std::filesystem::path::preferred_separator;
    result += segment2;
    result += std::filesystem::path::preferred_separator;
    result += segment3;
}

int SystemFilesReaderLinux::create_fd(const std::string& path)
{
    int fd = open(path.c_str(), O_RDONLY);
    if (fd == -1)
    {
        return -1;
    }
    return fd;
}

int SystemFilesReaderLinux::get_fd_or_throw(const std::string& file_path) const
{
    auto fd = m_fd_cache_ptr->get_by_file_path(file_path);
    check_fd_or_throw(fd, file_path);
    return fd.value();
}

void SystemFilesReaderLinux::check_fd_or_throw(const std::optional<int>& fd_opt, std::string_view name)
{
    if (!fd_opt.has_value())
    {
        std::stringstream ss;
        ss << "Unable to read data from closed fd: '" << name << '\'';
        throw std::runtime_error(ss.str());
    }
}
