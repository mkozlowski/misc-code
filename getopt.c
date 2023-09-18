#include <stdio.h>
#include <getopt.h>
#include <errno.h>

#include "common.h"

#define SPACE "    "

/*
 * More info on extern optarg, optind, opterr and optopt in getopt_core.h.
 */

int main(int argc, char *argv[])
{
	/*
	 * a - no argument
	 * b - required argument
	 * c - optional argument
	 * d - no argument
	 */
	const char *options = "ab:c::d";
	int opt_a = 0;
	int opt_b = 0;
	char *opt_b_arg = NULL;
	int opt_c = 0;
	char *opt_c_arg = NULL;
	int opt_d = 0;
	int i;

	log("getopt() for\n");
	log(SPACE "%s\n", options);

	log("args before getopt()\n");
	for (i = 0; i < argc; i++)
		log(SPACE "arg[%d -> %p] = %s\n", i, argv[i], argv[i]);

	log("opt??? before getopt()\n");
	log(SPACE "optarg: %s -> %p\n", optarg, optarg);
	log(SPACE "optind: %d\n", optind);
	log(SPACE "opterr: %d\n", opterr);
	log(SPACE "optopt: %c\n", optopt);

	log("getopt() loop\n");
	while ((i = getopt(argc, argv, options)) != -1) {
		log(SPACE "'%c' optarg: %s -> %p\n", i, optarg, optarg);
		log(SPACE "'%c' optind: %d\n", i, optind);
		log(SPACE "'%c' opterr: %d\n", i, opterr);
		log(SPACE "'%c' optopt: %c\n", i, optopt);

		switch (i) {
			case 'a':
				opt_a = 1;
				break;
			case 'b':
				opt_b = 1;
				opt_b_arg = optarg;
				break;
			case 'c':
				opt_c = 1;
				/* handle opt followed by arg */
				if (optarg)
					opt_c_arg = optarg;
				/* handle opt followed by space and then arg */
				else if (optind < argc && argv[optind][0] != '-')
					opt_c_arg = argv[optind++];
				break;
			case 'd':
				/* handle opt more than once */
				opt_d++;
				break;
			default:
				log(SPACE "unhandled case '%c'\n", i);
		}
	}

	log("opt??? after getopt()\n");
	log(SPACE "optarg: %s -> %p\n", optarg, optarg);
	log(SPACE "optind: %d\n", optind);
	log(SPACE "opterr: %d\n", opterr);
	log(SPACE "optopt: %c\n", optopt);

	log("parsed options\n");
	log(SPACE "a = %d\n", opt_a);
	log(SPACE "b = %d, arg %s -> %p\n", opt_b, opt_b_arg, opt_b_arg);
	log(SPACE "c = %d, arg %s -> %p\n", opt_c, opt_c_arg, opt_c_arg);
	log(SPACE "d = %d\n", opt_d);

	log("args after getopt()\n");
	for (i = 0; i < argc; i++)
		log(SPACE "arg[%d -> %p] = %s\n", i, argv[i], argv[i]);

	return 0;
}
