#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


/*
 * This library is intended to be preloaded so that it can abort at a specific
 * point and generate a core dump for analysis. It needs to be manually altered
 * to match specific case.
 *
 * Notes:
 *   -funwind-tables might be needed on some architectures (arm, riscv)
 *    for backtrace() to work
 *   -rdynamic can help make backtrace() output more readable
 *
 * Backtrace output can be further decoded with add2line:
 * $ addr2line -fp -e ./elf addr
 *
 */


#define DLSYM_CHECK(handle) do { \
	if ((handle) != NULL) { \
		/* fprintf(stdout, "libsymtrap.so: wrapping %s()\n", __func__); */ \
	} else { \
		fprintf(stderr, "%s\n", dlerror()); \
		abort(); \
	} \
} while (0)


static void do_abort(void)
{
	int size;
	void *buffer[64];

	size = backtrace(buffer, sizeof(buffer));
	fprintf(stderr, "backtrace() returned %d addresses:\n", size);
	backtrace_symbols_fd(buffer, size, STDERR_FILENO);
	abort();
}


#if 1
/*
 * extern int clock_gettime (clockid_t __clock_id, struct timespec *__tp) __THROW;
 */
int clock_gettime(clockid_t clock_id, struct timespec *tp)
{
	static int (*__clock_gettime)(clockid_t, struct timespec *) = NULL;
	static unsigned long count = 0;

	if (__clock_gettime == NULL) {
		__clock_gettime = (int (*)(clockid_t, struct timespec *))dlsym(RTLD_NEXT, "clock_gettime");
		DLSYM_CHECK(__clock_gettime);
	}

	/*
	 * app under certain load generated 3~5k calls / second so the idea was
	 * to abort but only when file flag exists
	 */
	if (++count % 4000 == 0) {
		int ret;

		ret = access("/tmp/symtrap_clock_gettime", F_OK);
		if (!ret)
			do_abort();
	}

	return __clock_gettime(clock_id, tp);
}
#endif

#if 1
/*
 * extern int ioctl (int __fd, unsigned long int __request, ...) __THROW;
 */
int ioctl(int fd, unsigned long int request, ...)
{
	int ret;
	va_list args;
	static int (*__ioctl)(int, unsigned long int, ...) = NULL;
	unsigned long int ioc;

	if (__ioctl == NULL) {
		__ioctl = (int (*)(int, unsigned long int, ...))dlsym(RTLD_NEXT, "ioctl");
		DLSYM_CHECK(__ioctl);
	}

	/*
	 * ioctl from strace:
	 * ioctl(11, _IOC(0, 0x64, 0x05, 0x65), 0xb60a4bb8);
	 */
	ioc = _IOC(0, 0x64, 0x05, 0x65);

	/* abort on matching ioctl */
	if (request == ioc)
		do_abort();

	va_start(args, request);
	ret = __ioctl(fd, request, args);
	va_end(args);

	return ret;
}
#endif

#if 1
/*
 * match this file name
 */
#define FILE_PATH "/dev/input/event1"

/*
 * extern int open (const char *__file, int __oflag, ...) __nonnull ((1));
 */
int open(const char *pathname, int flags, ...)
{
	int ret;
	va_list args;
	static int (*__open)(const char *, int, ...) = NULL;

	if (__open == NULL) {
		__open = (int (*)(const char *, int, ...))dlsym(RTLD_NEXT, "open");
		DLSYM_CHECK(__open);
	}

	/* abort on matching file name */
	if (!strcmp(pathname, FILE_PATH))
		do_abort();

	va_start(args, flags);
	ret = __open(pathname, flags, args);
	va_end(args);

	return ret;
}

/*
 * extern int open64 (const char *__file, int __oflag, ...) __nonnull ((1));
 */
int open64(const char *pathname, int flags, ...)
{
	int ret;
	va_list args;
	static int (*__open64)(const char *, int, ...) = NULL;

	if (__open64 == NULL) {
		__open64 = (int (*)(const char *, int, ...))dlsym(RTLD_NEXT, "open64");
		DLSYM_CHECK(__open64);
	}

	/* abort on matching file name */
	if (!strcmp(pathname, FILE_PATH))
		do_abort();

	va_start(args, flags);
	ret = __open64(pathname, flags, args);
	va_end(args);

	return ret;
}
#endif
