#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

#include "common.h"

/*
 * See bits/confname.h.
 */
#define ENTRY(e) { (e), #e }

struct conf_entry {
	int number;
	char *name;
} static conf[] = {
	ENTRY(_PC_LINK_MAX),
	ENTRY(_PC_MAX_CANON),
	ENTRY(_PC_MAX_INPUT),
	ENTRY(_PC_NAME_MAX),
	ENTRY(_PC_PATH_MAX),
	ENTRY(_PC_PIPE_BUF),
	ENTRY(_PC_CHOWN_RESTRICTED),
	ENTRY(_PC_NO_TRUNC),
	ENTRY(_PC_VDISABLE),
	ENTRY(_PC_SYNC_IO),
	ENTRY(_PC_ASYNC_IO),
	ENTRY(_PC_PRIO_IO),
	ENTRY(_PC_SOCK_MAXBUF),
	ENTRY(_PC_FILESIZEBITS),
	ENTRY(_PC_REC_INCR_XFER_SIZE),
	ENTRY(_PC_REC_MAX_XFER_SIZE),
	ENTRY(_PC_REC_MIN_XFER_SIZE),
	ENTRY(_PC_REC_XFER_ALIGN),
	ENTRY(_PC_ALLOC_SIZE_MIN),
	ENTRY(_PC_SYMLINK_MAX),
	ENTRY(_PC_2_SYMLINKS),
};

int main(int argc, char *argv[])
{
	int opt, i;
	int opt_brute_force = 0;
	const char *path;
	struct conf_entry *e;
	long val;

	while ((opt = getopt(argc, argv, "b")) != -1) {
		switch (opt) {
			case 'b':
				opt_brute_force = 1;
				break;
			default:
				exit(1);
		}
	}

	path = optind < argc ? argv[optind] : argv[0];

	if (opt_brute_force) {
		for (i = 0; i < 64 * 1024; i++) {
			errno = 0;
			val = pathconf(path, i);
			if (errno)
				continue;

			log("%05d = %ld\n", i, val);
		}

		return 0;
	}

	for (e = ARRAY_BEGIN(conf); e < ARRAY_END(conf); e++) {
		errno = 0;
		val = pathconf(path, e->number);
		if (errno)
			continue;

		log("%-23s %2d = %ld\n", e->name, e->number, val);
	}

	return 0;
}