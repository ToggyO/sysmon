#include <algorithm>
#include <filesystem>
#include <fstream>
#include <csignal> // _SC_CLK_TCK

#include "sys_info/process.hpp"
#include "../common_data_reader/common_data_reader_linux.hpp"
#include "../../common/constants.hpp" // k_colon_delimeter

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

// TODO: оптимизировать обращение к /proc/<pid>/stat
// 3 раза обращаюсь к файлу при формировании сущности Process
void Process::set_cpu_utilization(size_t process_id)
{
    if (is_cpu_utilization_set) { return; }

    // TODO: duplicate
    std::ostringstream oss;
    oss << k_proc_directory << pid << k_cmdline_filename;
    const auto source = oss.str();

    std::fstream stats_fs(source);
    check_fs_is_open_or_throw(stats_fs, source);

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
        if (this->is_uptime_set)
        {
            set_process_uptime(process_id);
        }
        this->cpu_usage = current_runtime / (double)this->uptime;
        is_cpu_utilization_set = true;
    }
}

void Process::set_command(const size_t process_id)
{
    if (is_command_set) { return; }

    // TODO: duplicate
    std::ostringstream oss;
    oss << k_proc_directory << pid << k_cmdline_filename;
    const auto source = oss.str();

    std::fstream cmdline_fs(source);
    check_fs_is_open_or_throw(cmdline_fs, source);

    std::getline(cmdline_fs, this->command);
    is_command_set = true;
}

void Process::set_uid(const size_t process_id, std::string &uid)
{
    // TODO: duplicate
    std::ostringstream oss;
    oss << k_proc_directory << process_id << k_proc_status_filename;
    const auto source = oss.str();

    std::fstream status_fs(source);
    check_fs_is_open_or_throw(status_fs, source);

    std::string line;
    std::string key;
    std::istringstream tokenizer;

    while (std::getline(status_fs, line))
    {
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, k_colon_delimeter);
        if (key != k_uid_key) { continue; }

        std::getline(tokenizer, uid, k_colon_delimeter);
        break;
    }
}

void Process::set_process_owner(const size_t process_id)
{
    if (is_user_set) { return; }

    std::fstream passwd_fs(k_pwd_path);
    check_fs_is_open_or_throw(passwd_fs, k_pwd_path);

    std::string line;
    std::istringstream iss;
    std::string uid;
    set_uid(process_id, uid);

    while (std::getline(passwd_fs, line))
    {
        if (line.find_first_of(uid) == std::string::npos) { continue; }

        std::replace(line.begin(), line.end(), ':', ' ');
        iss.clear();
        iss.str(line);
        iss >> this->user;
        is_user_set = true;
    }
}

void Process::set_process_uptime(const size_t process_id)
{
    if (is_uptime_set) { return; }

    std::ostringstream oss;
    oss << k_proc_directory << process_id << k_proc_stat_filename;
    const auto source = oss.str();

    std::fstream stats_fs(source);
    check_fs_is_open_or_throw(stats_fs, source);

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
        is_uptime_set = true;
    }
}

void Process::set_memory_usage(const size_t process_id)
{
    if (is_memory_usage_set) { return; }

    // TODO: duplicate
    std::ostringstream oss;
    oss << k_proc_directory << process_id << k_proc_status_filename;
    const auto source = oss.str();

    std::fstream status_fs(source);
    check_fs_is_open_or_throw(status_fs, source);

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

        std::getline(tokenizer, value, k_colon_delimeter);
        this->memory_usage = std::stod(value);
        is_memory_usage_set = true;
        break;
    }
}

bool Process::operator<(const Process & a) const
{
    return cpu_usage < a.cpu_usage;
}