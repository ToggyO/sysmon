#pragma once

#include <memory> // std::shared_ptr, std::weak_ptr
#include <string>
#include <fstream> // std::fstream
#include <sstream> // std::stringstream
#include <thread> // std::thread::hardware_concurrency
#include <vector>

#include "sys_info/cpu_load.hpp"
#include "../sys_info/cpu_stats.hpp"
#include "../filesystem/system_files_reader.interface.hpp"

/** @brief Represents functionality to read and handle CPU load */
class CpuReaderLinux
{
public:
    /** @brief Creates new instance of CpuReaderLinux
     *
     * @param files_reader_ptr Pointer to instance of ISystemFilesReader
    */
    explicit CpuReaderLinux(const std::shared_ptr<ISystemFilesReader>& files_reader_ptr);

    /** @brief Reads content of '/proc/stat' file, handle results and fill provided vector by CPU load info per core.
     * @static
     * @param Vector of @see CpuLoad
    */
    void read(std::vector<CpuLoad> &);

private:
    struct Jiffies
    {
        double active;
        double idle;
    };

    static void m_set_stats(std::istringstream &, CpuStats &);

    void m_read_cpu_data();

    const unsigned m_cpus_count;
    std::vector<CpuStats> m_measurements;
    std::vector<Jiffies> m_prev_results;
    std::weak_ptr<ISystemFilesReader> m_files_reader_ptr;
};
