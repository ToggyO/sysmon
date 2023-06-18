#pragma once

#include <string>
#include <fstream> // std::fstream
#include <sstream> // std::stringstream
#include <thread> // std::thread::hardware_concurrency
#include <vector>

#include "../../../include/sys_info/cpu_load.hpp"
#include "../../../include/sys_info/cpu_stats.hpp"
#include "../../common/error.h" // check_fs_is_open_or_throw
#include "../../common/stats_reader.hpp"

/** @brief Path to file, that keeps track of a variety of different statistics about the system since it was last restarted */
const static std::string m_source_name = "/proc/stat";
/** @brief Allows you to determine the presence of information about the loading of cpu cores when reading from the file /proc/stat */
const static std::string m_cpu_prefix = "cpu";

/** @brief Represents functionality to read and handle CPU load */
class CpuReaderLinux : public StatsReader<std::vector<CpuLoad>>
{
    public:
        /** @brief Creates new instance of @ {@link CpuReaderLinux}
         * @param size_t the delay between cpu stats collection
         */
        explicit CpuReaderLinux(size_t);

        /** @brief Reads content of '/proc/stat' file, handle results and fill provided vector by CPU load info per core.
         * @param Vector of @see CpuLoad
        */
        void read(std::vector<CpuLoad> &) override;

    private:
        static CpuStats m_create_stats(std::istringstream &);

        static void m_read_cpu_data(std::vector<CpuStats> &);

        static void m_calculate_cpu_load(
            const std::vector<CpuStats> &,
            const std::vector<CpuStats> &,
            std::vector<CpuLoad> &,
            size_t);

        const size_t m_cpu_usage_delay;
        const unsigned m_cpus_count;
        std::vector<CpuStats> m_first_measurement;
        std::vector<CpuStats> m_second_measurement;
};