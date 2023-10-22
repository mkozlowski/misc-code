#define _GNU_SOURCE

#include <stdio.h>
#include <sys/auxv.h>
#include <dlfcn.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "common.h"

static int (*__vdso_clock_gettime)(int clock_id, struct timespec *ts);
static int (*__vdso_clock_getres)(int clock_id, struct timespec *ts);
static int (*__vdso_gettimeofday)(struct timeval *tv, void *tz);
static int (*__vdso_getcpu)(unsigned int *cpu, unsigned int *node, void *unused);
#ifdef __NR_time
static time_t (*__vdso_time)(time_t *t);
#endif


static int sys_clock_gettime(void)
{
	int clock_id = CLOCK_MONOTONIC;
	struct timespec ts;

	return syscall(__NR_clock_gettime, clock_id, &ts);
}

static int vdso_clock_gettime(void)
{
	int clock_id = CLOCK_MONOTONIC;
	struct timespec ts;

	return __vdso_clock_gettime(clock_id, &ts);
}


static int sys_clock_getres(void)
{
	int clock_id = CLOCK_MONOTONIC;
	struct timespec ts;

	return syscall(__NR_clock_getres, clock_id, &ts);
}

static int vdso_clock_getres(void)
{
	int clock_id = CLOCK_MONOTONIC;
	struct timespec ts;

	return __vdso_clock_getres(clock_id, &ts);
}


static int sys_gettimeofday(void)
{
	struct timeval tv;

	return syscall(__NR_gettimeofday, &tv, NULL);
}

static int vdso_gettimeofday(void)
{
	struct timeval tv;

	return __vdso_gettimeofday(&tv, NULL);
}


static int sys_getcpu(void)
{
	unsigned int cpu, node;

	return syscall(__NR_getcpu, &cpu, &node, NULL);
}

static int vdso_getcpu(void)
{
	unsigned int cpu, node;

	return (int)__vdso_getcpu(&cpu, &node, NULL);
}


#ifdef __NR_time
static int sys_time(void)
{
	time_t t;

	return syscall(__NR_time, &t);
}

static int vdso_time(void)
{
	time_t t;

	return (int)__vdso_time(&t);
}
#endif


struct test {
	const char *desc;
	void *test;
	int (*sys)(void);
	int (*vdso)(void);
} static tests[] = {
	{
		.desc = "clock_gettime()",
		.test = &__vdso_clock_gettime,
		.sys  = sys_clock_gettime,
		.vdso = vdso_clock_gettime,
	}, {
		.desc = "clock_getres()",
		.test = &__vdso_clock_getres,
		.sys  = sys_clock_getres,
		.vdso = vdso_clock_getres,
	}, {
		.desc = "gettimeofday()",
		.test = &__vdso_gettimeofday,
		.sys  = sys_gettimeofday,
		.vdso = vdso_gettimeofday,
	}, {
		.desc = "getcpu()",
		.test = &__vdso_getcpu,
		.sys  = sys_getcpu,
		.vdso = vdso_getcpu,
	},
#ifdef __NR_time
	{
		.desc = "time()",
		.test = &__vdso_time,
		.sys  = sys_time,
		.vdso = vdso_time,
	},
#endif

};

static unsigned long long diff_ns(struct timespec a)
{
	struct timespec b;

	clock_gettime(CLOCK_MONOTONIC, &b);

	return (b.tv_sec * 1000000000ULL + b.tv_nsec) - (a.tv_sec * 1000000000ULL + a.tv_nsec);
}

static void do_benchmark(int (*func)(void), unsigned long *calls, unsigned long *diff)
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	for (;;) {
		for (int i = 0; i < 1000; i++)
			func();

		*calls += 1000;

		*diff = diff_ns(ts);
		if (*diff >= 250000000ULL)
			break;
	}
}

static void benchmark(void)
{
	struct test *t;

	for (t = ARRAY_BEGIN(tests); t < ARRAY_END(tests); t++) {
		unsigned long calls = 0;
		unsigned long diff = 0;
		double sys_time = 0;
		double vdso_time = 0;

		do_benchmark(t->sys, &calls, &diff);
		sys_time = (double)diff / calls;

		log("syscall %18s %9lu k calls in %.02f ms  %10.02f ns / call\n",
		    t->desc, calls / 1000, (double)diff / 1000000UL, sys_time);

		if (!*((unsigned long *)t->test)) {
			log("vDSO    %18s      not available\n", t->desc);
			continue;
		}

		do_benchmark(t->vdso, &calls, &diff);
		vdso_time = (double)diff / calls;

		log("vDSO    %18s %9lu k calls in %.02f ms  %10.02f ns / call (%.02fx faster)\n",
		    t->desc, calls / 1000, (double)diff / 1000000UL, vdso_time, sys_time / vdso_time);
	}
}

int main(int argc, char *argv[])
{
	int ret;
	unsigned long val;
	void *vdso;

	val = getauxval(AT_SYSINFO_EHDR);
	if (!val && errno == ENOENT)
		err("vDSO not available? vDSO address not found in auxiliary vector\n");
	else
		log("vDSO available @ 0x%lx\n", val);

	vdso = dlopen("linux-vdso.so.1", RTLD_LAZY);
	if (!vdso)
		die("dlopen() failed: %s\n", dlerror());

	__vdso_clock_gettime = dlsym(vdso, "__vdso_clock_gettime");
	__vdso_gettimeofday = dlsym(vdso, "__vdso_gettimeofday");
	__vdso_getcpu = dlsym(vdso, "__vdso_getcpu");
	__vdso_clock_getres = dlsym(vdso, "__vdso_clock_getres");
#ifdef __NR_time
	__vdso_time = dlsym(vdso, "__vdso_time");
#endif

	benchmark();

	ret = dlclose(vdso);
	if (ret)
		die("dlclose() failed: %s\n", dlerror());

	return 0;
}
