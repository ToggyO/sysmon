#pragma once

#include <string>
#include <fstream> // std::ifstream
#include <sstream> // std::stringstream
#include <thread> // std::thread::hardware_concurrency
#include <vector>

#include "../sys_info/cpu_load.hpp"
#include "../sys_info/cpu_stats.hpp"
#include "../common/error.h"

/** @brief Path to file, that keeps track of a variety of different statistics about the system since it was last restarted */
const static std::string m_source_name = "/proc/stat";
/** @brief Allows you to determine the presence of information about the loading of cpu cores when reading from the file /proc/stat */
const static std::string m_cpu_prefix = "cpu";

/** @brief Represents functionality to read and handle CPU load */
class CpuReaderLinux
{
    public:
        /** @brief Creates new instance of @ {@link CpuReaderLinux} */
        explicit CpuReaderLinux(size_t);

        /** @brief Reads content of '/proc/stat' file, handle results and fill provided vector by CPU load info per core.
         * @param Vector of @see {@link CpuLoad}
        */
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