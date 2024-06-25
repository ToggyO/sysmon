#include "system_monitor.hpp"

void set_stats(SystemInfo &system_info, const double averages[], int averages_count)
{
    auto *stats_p = (double *)&system_info.load_average_stats;
    for (int i = 0; i < averages_count; ++i)
    {
        *stats_p = averages[i];
        stats_p++;
    }
}

std::string SystemMonitor::collect_os_name()
{
    return m_common_data_reader->get_os_name();
}

void SystemMonitor::collect_cpu(SystemInfo &system_info)
{
    std::vector<CpuLoad> cpu_loads_collection;
    m_cpu_reader.read(cpu_loads_collection);
    system_info.cpu_load_collection = std::move(cpu_loads_collection);
}

void SystemMonitor::collect_load(SystemInfo &system_info)
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

void SystemMonitor::collect_memory(SystemInfo &system_info)
{
    MemoryStats mem_stats {};
    m_mem_reader.read(mem_stats);
    system_info.memory_stats = mem_stats;

}

void SystemMonitor::collect_processes_info(SystemInfo &system_info)
{
    std::vector<Process> processes;

    m_process_builder.build_processes(processes);
    std::sort(processes.begin(), processes.end(), [](const Process &p1, const Process &p2)
    {
        return p2 < p1;
    });

    system_info.processes = std::move(processes);
}

void SystemMonitor::collect_uptime(SystemInfo &system_info)
{
    system_info.uptime = m_common_data_reader->get_system_uptime();
}

// TODO: проверить присваивания , может сделать указатели в целях экономии памяти и времени выполнения
void SystemMonitor::collect_disks_usage(SystemInfo &system_info)
{
    // TODO: implement me in future
	// std::vector<std::unique_ptr<io_device>> devices;
	// std::vector<size_t> stats;

	// struct io_device iodvs;
	// struct io_stats ios;
	// std::istringstream iss;
    // for (const auto &dir_entry : std::filesystem::directory_iterator("/sys/block"))
    // {
	// 	stats.clear();
	// 	ios = std::make_unique<io_device>;
	// 	iodvs = std::make_unique<io_device>;

	// 	std::filesystem::path device_stats_path = dir_entry.path();
	// 	device_stats_path += "/stat";
    //     std::ifstream device_data(device_stats_path.make_preferred());
    //     if (!device_data.is_open())
    //     {
	// 		print_collection_error(dir_entry.path().native());
	// 		// TODO: duplicate
	// 		throw std::runtime_error("Cannot open: " + dir_entry.path().native());
    //     }

    //     std::string line;
    //     std::string line_segment;
    //     unsigned short counter = 0;

    //     while (std::getline(device_data, line))
    //     {
    //         iss.clear();
    //         iss.str(line);

    //         while (iss.good())
    //         {
    //             iss >> line_segment;
	// 			stats.push_back(std::move(std::stol(line_segment)));
    //         }
    //     }

	// 	size_t size = stats.size();
	// 	if (size >= 11)
	// 	{
	// 		/* Device or partition */
	// 		ios.rd_ios     = stats[0];
	// 		ios.rd_merges  = stats[1];
	// 		ios.rd_sectors = stats[2];
	// 		ios.rd_ticks   = stats[3];
	// 		ios.wr_ios     = stats[4];
	// 		ios.wr_merges  = stats[5];
	// 		ios.wr_sectors = stats[6];
	// 		ios.wr_ticks   = stats[7];
	// 		ios.ios_pgr    = stats[8];
	// 		ios.tot_ticks  = stats[9];
	// 		ios.rq_ticks   = stats[10];

	// 		if (size >= 15)
	// 		{
	// 			/* Discard I/O */
	// 			ios.dc_ios     = stats[11];
	// 			ios.dc_merges  = stats[12];
	// 			ios.dc_sectors = stats[13];
	// 			ios.dc_ticks   = stats[14];
	// 		}

	// 		if (size >= 17) {
	// 			/* Flush I/O */
	// 			ios.fl_ios     = stats[15];
	// 			ios.fl_ticks   = stats[16];
	// 		}
	// 	}

	// 	if (size == 4)
	// 	{
	// 		/* Partition without extended statistics */
	// 		ios.rd_ios     = stats[0];
	// 		ios.rd_sectors = stats[1];
	// 		ios.wr_ios     = stats[2];
	// 		ios.wr_sectors = stats[3];
	// 	}

	// 	iodvs.dev_stats[1] = ios;
	// 	devices.push_back(std::move(iodvs));
    // }

	// for (const auto &device : devices)
	// {
	// 	std::cout << device.name << std::endl;
	// }

	// system_info.disk_stats
}