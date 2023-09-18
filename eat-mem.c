#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "common.h"

static int interrupted;

static void *mem_malloc(size_t size)
{
	void *mem;

	log("allocate %zu bytes using malloc()\n", size);

	mem = malloc(size);
	if (!mem)
		die("malloc() failed: %m\n");

	return mem;
}

static void *mem_mmap(size_t size)
{
	void *mem;

	log("allocate %zu bytes using mmap()\n", size);

	mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (mem == MAP_FAILED)
		die("mmap() failed: %m\n");

	return mem;
}

static void mem_set(int random, int pattern, void *mem, size_t size)
{
	int ret;
	int fd;

	if (!random && pattern == -1)
		return;

	if (random) {
		log("random fill %zu bytes\n", size);

		/* read() limit, see man page */
		if (size > 0x7ffff000)
			die("size > 0x7ffff000\n");

		fd = open("/dev/urandom", O_RDONLY);
		if (fd == -1)
			die("open() failed: %m\n");

		ret = read(fd, mem, size);
		if (ret == -1)
			die("read() failed: %m\n");

		if (ret < size)
			err("read() short read %d < %zu\n", ret, size);

		close(fd);
	} else {
		log("pattern 0x%02x fill %zu bytes\n", pattern, size);
		memset(mem, pattern, size);
	}
}

#define STARTS_WITH(buf, str) !strncmp((buf), (str), strlen(str))

static void show_proc(void)
{
	FILE *fp;
	char buf[128];

	fputs("/proc/self/status\n", stdout);

	fp = fopen("/proc/self/status", "r");
	if (!fp)
		die("fopen() failed: %m\n");

	while (!feof(fp)) {
		fgets(buf, sizeof(buf), fp);

		if (STARTS_WITH(buf, "Name") ||
		    STARTS_WITH(buf, "Pid") ||
		    STARTS_WITH(buf, "Vm") ||
		    STARTS_WITH(buf, "Rss")) {
			fputs("    ", stdout);
			fputs(buf, stdout);
		}
	}

	fclose(fp);

	fputs("/proc/self/maps\n", stdout);

	fp = fopen("/proc/self/maps", "r");
	if (!fp)
		die("fopen() failed: %m\n");

	while (!feof(fp)) {
		fgets(buf, sizeof(buf), fp);
		fputs("    ", stdout);
		fputs(buf, stdout);
	}

	fclose(fp);
}

static void handle_signal(int signal)
{
	interrupted = signal;
}

static void usage(const char *name, int ret)
{
	log("%s [-h] [-m] [-p] [-P] [-r] -s size\n"
		"Opts:\n"
		"  -h this help info\n"
		"  -m use mmap\n"
		"  -p show /proc/<pid>/... info\n"
		"  -P pattern for memset 00..ff\n"
		"  -r fill allocated region with /dev/urandom data\n"
		"  -s allocation size\n", name);
	exit(ret);
}

int main(int argc, char *argv[])
{
	int opt;
	int opt_mmap = 0;
	int opt_proc = 0;
	int opt_pattern = -1;
	int opt_random = 0;
	long size = 0;
	int cnt = 0;
	char *endptr;
	int len;
	char *mem;

	while ((opt = getopt(argc, argv, "hmpP:rs:")) != -1) {
		switch (opt) {
			case 'h':
				usage(argv[0], 0);
				break;
			case 'm':
				opt_mmap = 1;
				break;
			case 'p':
				opt_proc = 1;
				break;
			case 'P':
				errno = 0;
				opt_pattern = strtol(optarg, &endptr, 16);
				if (errno)
					die("strtol() failed: %m\n");
				if (endptr == optarg)
					die("not a number: %s\n", optarg);
				if (opt_pattern < 0x00)
					opt_pattern = 0x00;
				if (opt_pattern > 0xff)
					opt_pattern = 0xff;
				break;
			case 'r':
				opt_random = 1;
				break;
			case 's':
				len = strlen(optarg);
				errno = 0;
				size = strtol(optarg, &endptr, 10);
				if (errno)
					die("strtol() failed: %m\n");
				if (endptr == argv[1])
					die("not a number: %s\n", optarg);
				if (optarg[len - 1] == 'k' || optarg[len - 1] == 'K')
					size *= 1024;
				else if (optarg[len - 1] == 'm' || optarg[len - 1] == 'M')
					size *= 1024 * 1024;
				else if (optarg[len - 1] == 'g' || optarg[len - 1] == 'G')
					size *= 1024 * 1024 * 1024;
				break;
			default:
				exit(1);
		}
	}

	if (size == 0)
		usage(argv[0], 1);

	if (opt_random && opt_pattern != -1)
		die("select either -r or -p\n");

	signal(SIGTERM, handle_signal);
	signal(SIGINT, handle_signal);

	if (opt_mmap)
		mem = mem_mmap(size);
	else
		mem = mem_malloc(size);

	mem_set(opt_random, opt_pattern, mem, size);

	if (opt_proc)
		show_proc();

	log("looping ...\n");

	while (!interrupted) {
		time_t now;
		struct tm *tm;
		char tbuf[64];

		now = time(NULL);
		tm = localtime(&now);
		strftime(tbuf, sizeof(tbuf), "%H:%M:%S", tm);

		log("%d %s %d\n", getpid(), tbuf, cnt);

		cnt++;
		sleep(1);
	}

	log("stop: %s\n", strsignal(interrupted));

	if (opt_mmap)
		munmap(mem, size);
	else
		free(mem);

	log("done\n");

	return 0;
}
