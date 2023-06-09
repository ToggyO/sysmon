#pragma once

#include <string>
#include <fstream> // std::ifstream
#include <sstream> // std::stringstream
#include <thread> // std::thread::hardware_concurrency
#include <vector>

#include "../sys_info/cpu_load.hpp"
#include "../sys_info/cpu_stats.hpp"
#include "../common/error.h"

const static std::string m_cpu_prefix = "cpu";

// TODO: add descr
class CpuReaderLinux
{
    public:
        explicit CpuReaderLinux(size_t);

        void read(std::vector<CpuLoad> &);

    private:
        CpuStats m_create_stats(const std::string &);

        void m_read_cpu_data(std::vector<CpuStats> &);

        void m_calculate_cpu_load(
            const std::vector<CpuStats> &,
            const std::vector<CpuStats> &,
            std::vector<CpuLoad> &,
            size_t);


        const size_t m_cpu_usage_delay;
};