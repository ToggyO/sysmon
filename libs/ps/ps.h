#pragma once

struct procps_status_t
{
	DIR *dir;
	DIR *task_dir;
	unsigned short shift_pages_to_bytes;
	unsigned short shift_pages_to_kb;
/* Fields are set to 0/NULL if failed to determine (or not requested) */
	unsigned short argv_len;
	char *argv0;
	char *exe;
	char *context;
	unsigned main_thread_pid;
	/* Everything below must contain no ptrs to malloc'ed data:
	 * it is memset(0) for each process in procps_scan() */
	unsigned long vsz, rss; /* we round it to kbytes */
	unsigned long stime, utime;
	unsigned long start_time;
	unsigned pid;
	unsigned ppid;
	unsigned pgid;
	unsigned sid;
	unsigned uid;
	unsigned gid;
	unsigned ruid;
	unsigned rgid;
	int niceness;
	unsigned tty_major,tty_minor;
	struct smaprec smaps;
	char state[4];
	/* basename of executable in exec(2), read from /proc/N/stat
	 * (if executable is symlink or script, it is NOT replaced
	 * by link target or interpreter name) */
	// char comm[COMM_LEN]; TODO:
	char comm[2];
	/* user/group? - use passwd/group parsing functions */
	int last_seen_on_cpu;
};

int run_ps();