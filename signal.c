#define _GNU_SOURCE /* strsignal(), sigabbrev_np(), sigdescr_np() */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "common.h"

#define ENTRY(e) [e] = #e

static char sig[_NSIG][16] = {
	ENTRY(SIGHUP),
	ENTRY(SIGINT),
	ENTRY(SIGQUIT),
	ENTRY(SIGILL),
	ENTRY(SIGTRAP),
	ENTRY(SIGABRT),
	ENTRY(SIGIOT),
	ENTRY(SIGBUS),
	ENTRY(SIGFPE),
	ENTRY(SIGKILL),
	ENTRY(SIGUSR1),
	ENTRY(SIGSEGV),
	ENTRY(SIGUSR2),
	ENTRY(SIGPIPE),
	ENTRY(SIGALRM),
	ENTRY(SIGTERM),
	ENTRY(SIGSTKFLT),
	ENTRY(SIGCHLD),
	ENTRY(SIGCONT),
	ENTRY(SIGSTOP),
	ENTRY(SIGTSTP),
	ENTRY(SIGTTIN),
	ENTRY(SIGTTOU),
	ENTRY(SIGURG),
	ENTRY(SIGXCPU),
	ENTRY(SIGXFSZ),
	ENTRY(SIGVTALRM),
	ENTRY(SIGPROF),
	ENTRY(SIGWINCH),
	ENTRY(SIGIO),
	ENTRY(SIGPOLL),
	ENTRY(SIGPWR),
	ENTRY(SIGSYS),
};


static void sig_rt_init(void)
{
	int i;

	for (i = SIGRTMIN; i <= SIGRTMAX; i++)
		snprintf(sig[i], sizeof(sig[i]), "SIGRT + %d", i - SIGRTMIN);
}

static void print_signal(int number)
{
	if (sig[number][0] == '\0') {
		log("unknown signal %d\n", number);
		return;
	}

#if __GLIBC_PREREQ(2, 32)
	log("%-11s %2d  %-25s %-8s %-25s\n", sig[number], number,
	    strsignal(number), sigabbrev_np(number), sigdescr_np(number));
#else
	log("%-11s %2d  %-25s\n", sig[number], number, strsignal(number));
#endif
}

int main(int argc, char *argv[])
{
	int i;
	char *endptr;
	long val;

	sig_rt_init();

	if (argc == 1) {
		for (i = 0; i < ARRAY_SIZE(sig); i++)
			print_signal(i);

		return 0;
	}

	if (argv[1][0] == 'S') {
		for (i = 0; i < ARRAY_SIZE(sig); i++)
			if (!strncmp(sig[i], argv[1], strlen(argv[1])))
				print_signal(i);

		return 0;
	}

	errno = 0;
	val = strtol(argv[1], &endptr, 10);
	if (errno)
		die("strtol() failed: %m\n");

	if (endptr == argv[1])
		die("not a number: %s\n", argv[1]);

	for (i = 0; i < ARRAY_SIZE(sig); i++) {
		if (i == val) {
			print_signal(i);
			return 0;
		}
	}

	die("signal %ld not found\n", val);
}
