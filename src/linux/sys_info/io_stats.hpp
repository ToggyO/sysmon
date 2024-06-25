#pragma once

#include <string>
#include <sys/statvfs.h>

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
