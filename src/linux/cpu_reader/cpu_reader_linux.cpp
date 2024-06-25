#include "cpu_reader_linux.hpp"
#include "../linux_constants.hpp" // m_source_name, m_cpu_prefix

CpuReaderLinux::CpuReaderLinux(ISystemFilesReader *files_reader)
    : m_cpus_count{std::thread::hardware_concurrency()},
    m_measurements{},
    m_prev_results{},
    m_files_reader{files_reader}
{
    m_measurements.reserve(m_cpus_count);
    m_prev_results.reserve(m_cpus_count);

    for (int i = 0; i < m_cpus_count; ++i) // Initial values
    {
        m_prev_results.emplace_back();
    }
}

void CpuReaderLinux::read(std::vector<CpuLoad> &cpu_loads_collection)
{
    m_measurements.clear();

    m_read_cpu_data();

     for (size_t i = 0; i < m_cpus_count; i++)
    {
        const auto &actual = m_measurements[i];
        auto &previous = m_prev_results[i];

        // calculate delta jiffies
        const double d_active_time = (double)actual.get_total_active() - previous.active;
        const double d_idle_time =  (double)actual.get_total_idle() - previous.idle;
        const double d_total_time = d_active_time + d_idle_time;

        double percentage = (d_active_time / d_total_time);
        cpu_loads_collection.push_back(CpuLoad{std::to_string(i),percentage});

        previous.active = (double)actual.get_total_active();
        previous.idle = (double)actual.get_total_idle();
    }
}

void CpuReaderLinux::m_set_stats(std::istringstream &iss, CpuStats &cpu_stats)
{
    iss >> cpu_stats.cpu_id;

    auto *stats_p = (size_t*)&cpu_stats;
    while (iss >> *stats_p)
    {
        stats_p++;
    }
}

void CpuReaderLinux::m_read_cpu_data()
{
    CpuStats cpu_stats{};

    std::stringstream proc_stat;
    m_files_reader->read_proc_stat(proc_stat);

    std::string line;
    std::istringstream ss;
    getline(proc_stat, line); // skip first line with cpu summary

    while (getline(proc_stat, line))
    {
        ss.clear();
        ss.str(line);
        auto k = line.rfind(LinuxConstants::m_cpu_prefix, 0);
        if (k != 0)
        {
            break;
        }
        CpuStats stats{};
        m_set_stats(ss, stats);
        m_measurements.push_back(std::move(stats));
    }
}