#include "cpu_reader_linux.hpp"

CpuReaderLinux::CpuReaderLinux(size_t cpu_usage_delay)
    : m_cpu_usage_delay{cpu_usage_delay},
    m_cpus_count{std::thread::hardware_concurrency()},
    m_first_measurement{},
    m_second_measurement{}
{
    m_first_measurement.reserve(m_cpus_count);
    m_second_measurement.reserve(m_cpus_count);
}

void CpuReaderLinux::read(std::vector<CpuLoad> &cpu_loads_collection)
{
    m_first_measurement.clear();
    m_second_measurement.clear();
    // auto cpus_count = std::thread::hardware_concurrency();

    // auto first_measurement = std::vector<CpuStats>();
    // first_measurement.reserve(m_cpus_count);
    // auto second_measurement = std::vector<CpuStats>();
    // second_measurement.reserve(m_cpus_count);

    m_read_cpu_data(m_first_measurement);

    std::this_thread::sleep_for(std::chrono::microseconds(m_cpu_usage_delay));

    m_read_cpu_data(m_second_measurement);

    m_calculate_cpu_load(m_first_measurement, m_second_measurement, cpu_loads_collection, m_cpus_count);
}

CpuStats CpuReaderLinux::m_create_stats(std::istringstream &iss)
{
    CpuStats cpu_stats{};
    iss >> cpu_stats.cpu_id;

    auto *stats_p = (size_t*)&cpu_stats;
    while (iss >> *stats_p)
    {
        stats_p++;
    }

    return cpu_stats;
}

void CpuReaderLinux::m_read_cpu_data(std::vector<CpuStats> &stats_collection)
{
    CpuStats cpu_stats{};

    std::fstream proc_stat(m_source_name);
    check_fs_is_open_or_throw(proc_stat, m_source_name);

    std::string line;
    std::istringstream ss;
    getline(proc_stat, line); // skip first line with cpu summary

    while (getline(proc_stat, line))
    {
        ss.clear();
        ss.str(line); // TODO: check
//        ss.seekg(0, std::ios::beg); // TODO: check ss
        auto k = line.rfind(m_cpu_prefix, 0);
        if (k != 0)
        {
            break;
        }
        stats_collection.push_back(std::move(m_create_stats(ss)));
    }
}

void CpuReaderLinux::m_calculate_cpu_load(
    const std::vector<CpuStats> &first_measurement,
    const std::vector<CpuStats> &second_measurement,
    std::vector<CpuLoad> &cpu_loads_collection,
    size_t cpu_count)
{
    for (size_t i = 0; i < cpu_count; i++)
    {
        const auto first = first_measurement[i];
        const auto second = second_measurement[i];

        const double active_time = second.get_total_active() - first.get_total_active();
        const double idle_time = second.get_total_idle() - first.get_total_idle();
        const double total_time = active_time + idle_time;

        double k = (active_time / total_time) * 100;
        cpu_loads_collection.push_back(CpuLoad{first.cpu_id,k});
    }
}