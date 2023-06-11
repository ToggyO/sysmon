#include "cpu_reader.hpp"

CpuReaderLinux::CpuReaderLinux(size_t cpu_usage_delay) : m_cpu_usage_delay{cpu_usage_delay}
{}

void CpuReaderLinux::read(std::vector<CpuLoad> &cpu_loads_collection)
{
    auto cpus_count = std::thread::hardware_concurrency();

    auto first_measurement = std::vector<CpuStats>();
    first_measurement.reserve(cpus_count);
    auto second_measurement = std::vector<CpuStats>();
    second_measurement.reserve(cpus_count);

    m_read_cpu_data(first_measurement);

    std::this_thread::sleep_for(std::chrono::microseconds(m_cpu_usage_delay));

    m_read_cpu_data(second_measurement);

    m_calculate_cpu_load(first_measurement, second_measurement, cpu_loads_collection, cpus_count);
}

CpuStats CpuReaderLinux::m_create_stats(const std::string &line)
{
    CpuStats cpu_stats{};
    std::stringstream ss(line);
    ss >> cpu_stats.cpu_id;

    auto *stats_p = (size_t*)&cpu_stats;
    while (ss >> *stats_p)
    {
        stats_p++;
    }

    return cpu_stats;
}

void CpuReaderLinux::m_read_cpu_data(std::vector<CpuStats> &stats_collection)
{
    CpuStats cpu_stats{};

    std::ifstream proc_stat(m_source_name);
    if (!proc_stat.is_open())
    {
        print_collection_error(m_source_name);
        // TODO: duplicate
        throw std::runtime_error("Cannot open: " + m_source_name);
    }

    std::string line;
    getline(proc_stat, line); // skip first line with cpu summary

    while (getline(proc_stat, line))
    {
        auto k = line.rfind(m_cpu_prefix, 0);
        if (k != 0)
        {
            break;
        }
        stats_collection.push_back(std::move(m_create_stats(line)));
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