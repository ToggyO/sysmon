#include <algorithm>
#include <filesystem>
#include <fstream>
#include <csignal> // _SC_CLK_TCK

#include "sys_info/process.hpp"
#include "../common_data_reader/common_data_reader_linux.hpp"
#include "../../common/constants.hpp" // k_colon_delimeter

#include "../linux_constants.hpp" // k_proc_directory, k_proc_stat_filename, k_utime_index, k_stime_index, k_cutime_index, k_cstime_index, k_cmdline_filename, k_pwd_path, k_proc_status_filename, k_proc_stat_start_time_index, k_vm_size_key, k_uid_key

void create_path_from_segments(
    std::filesystem::path &result,
    const std::string &segment1,
    const std::string &segment2,
    const std::string &segment3)
{
    result += segment1;
    result += std::filesystem::path::preferred_separator;
    result += segment2;
    result += std::filesystem::path::preferred_separator;
    result += segment3;
}

Process::Process(size_t process_id) : pid{process_id}
{
    calculate_process_uptime();
    calculate_cpu_utilization();
    calculate_command();
    calculate_memory_usage();
    calculate_process_owner();
}

// TODO: оптимизировать обращение к /proc/<pid>/stat
// 3 раза обращаюсь к файлу при формировании сущности Process
void Process::calculate_cpu_utilization()
{
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(proc_cpu_file_path, k_proc_directory, std::to_string(pid), k_proc_stat_filename);

    std::ifstream stats_fs(proc_cpu_file_path);
    check_fs_is_open_or_throw(stats_fs, proc_cpu_file_path);

    double raw_cpu_usage = 0;
    std::string line;
    std::string temp_value;
    if (std::getline(stats_fs, line))
    {
        std::istringstream iss{line};
        for (int i = 0; i <= k_cstime_index; ++i)
        {
            iss >> temp_value;
            switch (i)
            {
                case k_utime_index:
                case k_stime_index:
                case k_cutime_index:
                case k_cstime_index:
                    raw_cpu_usage += std::stod(temp_value);
                    break;
                default:
                    break;
            }
        }

        const double current_runtime = raw_cpu_usage / (double)sysconf(_SC_CLK_TCK);
        this->cpu_usage = current_runtime / (double)this->uptime;
    }
}

void Process::calculate_command()
{
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(proc_cpu_file_path, k_proc_directory, std::to_string(pid), k_cmdline_filename);

    std::ifstream cmdline_fs(proc_cpu_file_path);
    check_fs_is_open_or_throw(cmdline_fs, proc_cpu_file_path);

    std::getline(cmdline_fs, this->command);
}

void Process::calculate_process_owner()
{
    std::ifstream passwd_fs(k_pwd_path);
    check_fs_is_open_or_throw(passwd_fs, k_pwd_path);

    std::string line;
    std::istringstream iss;
    std::string uid;
    set_uid(pid, uid);

    while (std::getline(passwd_fs, line))
    {
        if (line.find_first_of(uid) == std::string::npos) { continue; }

        std::replace(line.begin(), line.end(), ':', ' ');
        iss.clear();
        iss.str(line);
        iss >> this->user;
    }
}

void Process::calculate_process_uptime()
{
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(proc_cpu_file_path, k_proc_directory, std::to_string(pid), k_proc_stat_filename);

    std::ifstream stats_fs(proc_cpu_file_path);
    check_fs_is_open_or_throw(stats_fs, proc_cpu_file_path);

    std::string line;
    std::string temp_value;
    if (std::getline(stats_fs, line))
    {
        std::istringstream iss{line};
        for (int i = 0; i <= k_proc_stat_start_time_index; ++i)
        {
            iss >> temp_value;
        }

        size_t start_time = std::stol(temp_value) / sysconf(_SC_CLK_TCK);
        this->uptime = CommonDataReaderLinux::get_system_uptime() - start_time;
    }
}

void Process::calculate_memory_usage()
{
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(proc_cpu_file_path, k_proc_directory, std::to_string(pid), k_proc_status_filename);

    std::ifstream status_fs(proc_cpu_file_path);
    check_fs_is_open_or_throw(status_fs, proc_cpu_file_path);

    std::string line;
    std::string key;
    std::string value;
    std::istringstream tokenizer;

    while (std::getline(status_fs, line))
    {
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, k_colon_delimeter);
        if (key != k_vm_size_key) { continue; }

        std::getline(tokenizer, value);
        // TODO: raw value is like 'Key:\t   14431232 kB'. std::stod extract digits from it. But, it's too hard to rely on that
        // Need to parse raw value manually
        this->memory_usage = std::stod(value);
        break;
    }
}

void Process::set_uid(const size_t process_id, std::string &uid)
{
    std::filesystem::path proc_cpu_file_path;
    create_path_from_segments(proc_cpu_file_path, k_proc_directory, std::to_string(process_id), k_proc_status_filename);

    std::ifstream status_fs(proc_cpu_file_path);
    check_fs_is_open_or_throw(status_fs, proc_cpu_file_path);

    std::string line;
    std::string key;
    std::istringstream tokenizer;

    while (std::getline(status_fs, line))
    {
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, k_colon_delimeter);
        if (key != k_uid_key) { continue; }

        std::getline(tokenizer, uid);
        break;
    }
}

bool Process::operator<(const Process & a) const
{
    return cpu_usage < a.cpu_usage;
}