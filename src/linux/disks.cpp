#include <filesystem>
#include <iostream>
#include <sys/statvfs.h>
#include <fstream>
#include <vector>

#include "../common/error.h" // print_collection_error
#include "../common/system_monitor.hpp"

// TODO: check
struct io_stats
{
    /* # of sectors read */
	unsigned long rd_sectors;
	/* # of sectors written */
	unsigned long wr_sectors;
	/* # of sectors discarded */
	unsigned long dc_sectors;
	/* # of read operations issued to the device */
	unsigned long rd_ios;
	/* # of read requests merged */
	unsigned long rd_merges;
	/* # of write operations issued to the device */
	unsigned long wr_ios;
	/* # of write requests merged */
	unsigned long wr_merges;
	/* # of discard operations issued to the device */
	unsigned long dc_ios;
	/* # of discard requests merged */
	unsigned long dc_merges;
	/* # of flush requests issued to the device */
	unsigned long fl_ios;
	/* Time of read requests in queue */
	unsigned int  rd_ticks;
	/* Time of write requests in queue */
	unsigned int  wr_ticks;
	/* Time of discard requests in queue */
	unsigned int  dc_ticks;
	/* Time of flush requests in queue */
	unsigned int  fl_ticks;
	/* # of I/Os in progress */
	unsigned int  ios_pgr;
	/* # of ticks total (for this device) for I/O */
	unsigned int  tot_ticks;
	/* # of ticks requests spent in queue */
	unsigned int  rq_ticks;
};

struct io_device {
	std::string name;
	/*
	 * 0: Not a whole device (T_PART)
	 * 1: whole device (T_DEV)
	 * 2: whole device and all its partitions to be read (T_PART_DEV)
	 * 3+: group name (T_GROUP) (4 means 1 device in the group, 5 means 2 devices in the group, etc.)
	 */
	int dev_tp;
	/* TRUE if device exists in /proc/diskstats or /sys. Don't apply for groups. */
	int exist;
	/* major and minor numbers (not set for T_GROUP "devices") */
	int major;
	int minor;
	struct io_stats *dev_stats[2];
	struct io_device *next;
};

// TODO: проверить присваивания , может сделать указатели в целях экономии памяти и времени выполнения
void SystemMonitor::collect_disks_usage(SystemInfo &system_info)
{
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