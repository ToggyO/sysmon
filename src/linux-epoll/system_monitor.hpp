#pragma once

#include <fcntl.h> // open
#include <fstream> // std::ifstream
#include <memory> // std::shared_ptr
#include <vector> // std::vector
#include <stdexcept> // std::runtime_error
#include <string> // std::string
#include <sstream> // std::stringstream

#include "poller.interface.hpp"
#include "sys_info/system_info.hpp"
#include "../linux/linux_constants.hpp" // TODO: перенести // LinuxConstants::k_proc_stat_file
#include "../common/constants.hpp" // Constants::k_equal_sign_delimiter

// TODO: add descr
// TODO: разгрузить SystemMonitor, разделить методы на разные классы
class SystemMonitor
{
public:
    explicit SystemMonitor(const std::shared_ptr<IPoller>& poller_ptr)
        : m_poller_ptr{poller_ptr}
    {}

    void collect(SystemInfo &system_info)
    {
        auto poller = m_poller_ptr.lock();
        if (!poller)
        {
            throw std::runtime_error("Poller is required");
        }

        if (!system_info.is_os_name_set())
        {
            system_info.set_os_name(get_os_name());
        }

        collect_load(system_info); // TODO: IN DA LOOP

        // size_t static_fds_count; // TODO: ПОСЧИТАТЬ
        // size_t dynamic_fds_count;

        std::vector<int> static_fds;
        std::vector<int> dynamic_fds;

        // static_fds.reserve(static_fds_count);
        // dynamic_fds.reserve(dynamic_fds_count);

        static_fds.push_back(get_fd(LinuxConstants::k_proc_stat_file)); // Get "/proc/stat" file descriptor

         // TODO: написать еще один поллер, который будет доносить новые дескрипторы в отдельном потоке для процессов
        poller->update_fds(std::move(static_fds));
        poller->poll();
    }

private:
    std::weak_ptr<IPoller> m_poller_ptr;

    void collect_load(SystemInfo &system_info)
    {
        double empty = 0.00;
        int averages_count = 3;
        double averages[] { empty, empty, empty };
        if (getloadavg(averages, averages_count) < 0)
        {
            set_stats(system_info, averages, averages_count);
            return;
        }

        set_stats(system_info, averages, averages_count);
    }

    void set_stats(SystemInfo &system_info, const double averages[], int averages_count)
    {
        auto *stats_p = (double *)&system_info.load_average_stats;
        for (int i = 0; i < averages_count; ++i)
        {
            *stats_p = averages[i];
            stats_p++;
        }
    }

    // void collect_load_info_fds(std::vector<int>)
    // {
    //     int fd = open(LinuxConstants::k_proc_stat_file.c_str(), O_RDONLY);
    // }
    int get_fd(const std::string& file_path)
    {
        return open(LinuxConstants::k_proc_stat_file.c_str(), O_RDONLY);
    }

    std::string get_os_name() const
    {
        std::stringstream ss;
        std::ifstream os_fs(LinuxConstants::k_os_file_path);
        if (!os_fs.is_open())
        {
            throw std::runtime_error("Can't open file: " + LinuxConstants::k_os_file_path);
        }
        //  TODO: Поч закомментил?
    //    check_fs_is_open_or_throw(os_fs, LinuxConstants::k_os_file_path);

        ss << os_fs.rdbuf();
        os_fs.close();

        std::string line;
        std::string key;
        std::string value;
        std::istringstream tokenizer;

        while (std::getline(ss, line))
        {
            tokenizer.clear();
            tokenizer.str(line);

            std::getline(tokenizer, key, Constants::k_equal_sign_delimiter);
            if (key != LinuxConstants::k_pretty_name_key) { continue; }

            std::getline(tokenizer, value);
            break;
        }

        return value;
    }

};
