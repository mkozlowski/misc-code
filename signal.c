#define _GNU_SOURCE /* strsignal(), sigabbrev_np(), sigdescr_np() */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

#include "common.h"

#define ENTRY(e) { (e), #e }
#define ENTRY_RT(e, n) (struct signal_entry){ (e), (n) };

struct signal_entry {
	int number;
	char *name;
} static sig[] = {
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

static void print_signal(struct signal_entry *s)
{
	log("%-11s %2d  %-25s %-8s %-25s\n", s->name, s->number,
	    strsignal(s->number), sigabbrev_np(s->number), sigdescr_np(s->number));
}

int main(int argc, char *argv[])
{
	/*
	 * SIGRTMIN and SIGRTMAX are defined as __libc_current_sigrtmin()
	 * and __libc_current_sigrtmax() respecively hence separate sig_rt
	 * array.
	 */
	struct signal_entry sig_rt[SIGRTMAX - SIGRTMIN + 1];
	struct signal_entry *e;
	char *endptr;
	long val;

	for (e = ARRAY_BEGIN(sig_rt); e < ARRAY_END(sig_rt); e++) {
		char *name;

		/* yes, this is a leak */
		name = malloc(16);

		snprintf(name, 16, "SIGRT + %td", e - ARRAY_BEGIN(sig_rt));
		*e = ENTRY_RT(SIGRTMIN + e - ARRAY_BEGIN(sig_rt), name);
	}

	if (argc == 1) {
		for (e = ARRAY_BEGIN(sig); e < ARRAY_END(sig); e++)
			print_signal(e);

		for (e = ARRAY_BEGIN(sig_rt); e < ARRAY_END(sig_rt); e++)
			print_signal(e);

		return 0;
	}

	if (argv[1][0] == 'S') {
		for (e = ARRAY_BEGIN(sig); e < ARRAY_END(sig); e++)
			if (!strncmp(e->name, argv[1], strlen(argv[1])))
				print_signal(e);

		for (e = ARRAY_BEGIN(sig_rt); e < ARRAY_END(sig_rt); e++)
			if (!strncmp(e->name, argv[1], strlen(argv[1])))
				print_signal(e);

		return 0;
	}

	errno = 0;
	val = strtol(argv[1], &endptr, 10);
	if (errno)
		die("strtol() failed: %m\n");

	if (endptr == argv[1])
		die("not a number: %s\n", argv[1]);

	for (e = ARRAY_BEGIN(sig); e < ARRAY_END(sig); e++) {
		if (e->number == val) {
			print_signal(e);
			return 0;
		}
	}

	for (e = ARRAY_BEGIN(sig_rt); e < ARRAY_END(sig_rt); e++) {
		if (e->number == val) {
			print_signal(e);
			return 0;
		}
	}

	die("signal %ld not found\n", val);
}
