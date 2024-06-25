#include "proccess_builder_linux.hpp"

// TODO: оптимизировать обращение к файлу /proc/<pid>/status (read_process_status)

ProcessBuilderLinux::ProcessBuilderLinux(ISystemFilesReader *file_reader, CommonDataReaderLinux *common_data_reader)
    : m_file_reader{file_reader},
      m_common_data_reader{common_data_reader}
{}

void ProcessBuilderLinux::build_processes(std::vector<Process> &processes)
{
    for (const auto &dir_entry : std::filesystem::directory_iterator(LinuxConstants::k_proc_directory))
    {
        if (!dir_entry.is_directory()) { continue; }

        const auto name = dir_entry.path().filename().string();
        if (!std::all_of(name.begin(), name.end(), isdigit)) { continue; }

        Process process((size_t)std::stoi(name));

        this->calculate_cpu_utilization_and_uptime(process);
        this->calculate_command(process);
        this->calculate_memory_usage(process);
        this->calculate_process_owner(process);

        processes.push_back(process);
    }
}

void ProcessBuilderLinux::calculate_cpu_utilization_and_uptime(Process &process)
{
    std::stringstream ss;
    m_file_reader->read_process_stat(ss, process.pid);

    double raw_cpu_usage = 0;
    std::string line;
    std::string temp_value;

    if (!std::getline(ss, line)) { return; }

    std::istringstream iss{line};
    for (int i = 0; i <= LinuxConstants::k_proc_stat_start_time_index; ++i)
    {
        iss >> temp_value;
        switch (i)
        {
            case LinuxConstants::k_utime_index:
            case LinuxConstants::k_stime_index:
            case LinuxConstants::k_cutime_index:
            case LinuxConstants::k_cstime_index:
                raw_cpu_usage += std::stod(temp_value);
                break;
            case LinuxConstants::k_proc_stat_start_time_index:
            {
                size_t start_time = std::stol(temp_value) / sysconf(_SC_CLK_TCK);
                process.uptime = m_common_data_reader->get_system_uptime() - start_time;
                break;
            }
            default:
                break;
        }
    }

    const double current_runtime = raw_cpu_usage / (double)sysconf(_SC_CLK_TCK);
    process.cpu_usage = current_runtime / (double)process.uptime;
}

void ProcessBuilderLinux::calculate_command(Process &process)
{
    std::stringstream ss;
    m_file_reader->read_process_cmdline(ss, process.pid);
    std::getline(ss, process.command);
}

void ProcessBuilderLinux::calculate_process_owner(Process &process)
{
    std::stringstream ss;
    m_file_reader->read_process_etc_passwd(ss, process.pid);

    std::string line;
    std::istringstream iss;
    std::string uid;
    set_uid(process.pid, uid);

    while (std::getline(ss, line))
    {
        if (line.find_first_of(uid) == std::string::npos) { continue; }

        std::replace(line.begin(), line.end(), ':', ' ');
        iss.clear();
        iss.str(line);
        iss >> process.user;
    }
}

void ProcessBuilderLinux::calculate_memory_usage(Process &process)
{
    std::stringstream ss;
    m_file_reader->read_process_status(ss, process.pid);

    std::string line;
    std::string key;
    std::string value;
    std::istringstream tokenizer;

    while (std::getline(ss, line))
    {
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, Constants::k_colon_delimiter);
        if (key != LinuxConstants::k_vm_size_key) { continue; }

        std::getline(tokenizer, value);
        // TODO: raw value is like 'Key:\t   14431232 kB'. std::stod extract digits from it. But, it's too hard to rely on that
        // Need to parse raw value manually
        process.memory_usage = std::stod(value);
        break;
    }
}

void ProcessBuilderLinux::set_uid(const size_t &process_id, std::string &uid)
{
    std::stringstream ss;
    m_file_reader->read_process_status(ss, process_id);

    std::string line;
    std::string key;
    std::istringstream tokenizer;
    while (std::getline(ss, line))
    {
        tokenizer.clear();
        tokenizer.str(line);

        std::getline(tokenizer, key, Constants::k_colon_delimiter);
        if (key != LinuxConstants::k_uid_key) { continue; }

        std::getline(tokenizer, uid);
        break;
    }
}
