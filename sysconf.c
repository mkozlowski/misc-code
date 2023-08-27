#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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
	ENTRY(_SC_ARG_MAX),
	ENTRY(_SC_CHILD_MAX),
	ENTRY(_SC_CLK_TCK),
	ENTRY(_SC_NGROUPS_MAX),
	ENTRY(_SC_OPEN_MAX),
	ENTRY(_SC_STREAM_MAX),
	ENTRY(_SC_TZNAME_MAX),
	ENTRY(_SC_JOB_CONTROL),
	ENTRY(_SC_SAVED_IDS),
	ENTRY(_SC_REALTIME_SIGNALS),
	ENTRY(_SC_PRIORITY_SCHEDULING),
	ENTRY(_SC_TIMERS),
	ENTRY(_SC_ASYNCHRONOUS_IO),
	ENTRY(_SC_PRIORITIZED_IO),
	ENTRY(_SC_SYNCHRONIZED_IO),
	ENTRY(_SC_FSYNC),
	ENTRY(_SC_MAPPED_FILES),
	ENTRY(_SC_MEMLOCK),
	ENTRY(_SC_MEMLOCK_RANGE),
	ENTRY(_SC_MEMORY_PROTECTION),
	ENTRY(_SC_MESSAGE_PASSING),
	ENTRY(_SC_SEMAPHORES),
	ENTRY(_SC_SHARED_MEMORY_OBJECTS),
	ENTRY(_SC_AIO_LISTIO_MAX),
	ENTRY(_SC_AIO_MAX),
	ENTRY(_SC_AIO_PRIO_DELTA_MAX),
	ENTRY(_SC_DELAYTIMER_MAX),
	ENTRY(_SC_MQ_OPEN_MAX),
	ENTRY(_SC_MQ_PRIO_MAX),
	ENTRY(_SC_VERSION),
	ENTRY(_SC_PAGESIZE),
	ENTRY(_SC_RTSIG_MAX),
	ENTRY(_SC_SEM_NSEMS_MAX),
	ENTRY(_SC_SEM_VALUE_MAX),
	ENTRY(_SC_SIGQUEUE_MAX),
	ENTRY(_SC_TIMER_MAX),
	ENTRY(_SC_BC_BASE_MAX),
	ENTRY(_SC_BC_DIM_MAX),
	ENTRY(_SC_BC_SCALE_MAX),
	ENTRY(_SC_BC_STRING_MAX),
	ENTRY(_SC_COLL_WEIGHTS_MAX),
	ENTRY(_SC_EQUIV_CLASS_MAX),
	ENTRY(_SC_EXPR_NEST_MAX),
	ENTRY(_SC_LINE_MAX),
	ENTRY(_SC_RE_DUP_MAX),
	ENTRY(_SC_CHARCLASS_NAME_MAX),
	ENTRY(_SC_2_VERSION),
	ENTRY(_SC_2_C_BIND),
	ENTRY(_SC_2_C_DEV),
	ENTRY(_SC_2_FORT_DEV),
	ENTRY(_SC_2_FORT_RUN),
	ENTRY(_SC_2_SW_DEV),
	ENTRY(_SC_2_LOCALEDEF),
	ENTRY(_SC_PII),
	ENTRY(_SC_PII_XTI),
	ENTRY(_SC_PII_SOCKET),
	ENTRY(_SC_PII_INTERNET),
	ENTRY(_SC_PII_OSI),
	ENTRY(_SC_POLL),
	ENTRY(_SC_SELECT),
	ENTRY(_SC_UIO_MAXIOV),
	ENTRY(_SC_IOV_MAX),
	ENTRY(_SC_PII_INTERNET_STREAM),
	ENTRY(_SC_PII_INTERNET_DGRAM),
	ENTRY(_SC_PII_OSI_COTS),
	ENTRY(_SC_PII_OSI_CLTS),
	ENTRY(_SC_PII_OSI_M),
	ENTRY(_SC_T_IOV_MAX),
	ENTRY(_SC_THREADS),
	ENTRY(_SC_THREAD_SAFE_FUNCTIONS),
	ENTRY(_SC_GETGR_R_SIZE_MAX),
	ENTRY(_SC_GETPW_R_SIZE_MAX),
	ENTRY(_SC_LOGIN_NAME_MAX),
	ENTRY(_SC_TTY_NAME_MAX),
	ENTRY(_SC_THREAD_DESTRUCTOR_ITERATIONS),
	ENTRY(_SC_THREAD_KEYS_MAX),
	ENTRY(_SC_THREAD_STACK_MIN),
	ENTRY(_SC_THREAD_THREADS_MAX),
	ENTRY(_SC_THREAD_ATTR_STACKADDR),
	ENTRY(_SC_THREAD_ATTR_STACKSIZE),
	ENTRY(_SC_THREAD_PRIORITY_SCHEDULING),
	ENTRY(_SC_THREAD_PRIO_INHERIT),
	ENTRY(_SC_THREAD_PRIO_PROTECT),
	ENTRY(_SC_THREAD_PROCESS_SHARED),
	ENTRY(_SC_NPROCESSORS_CONF),
	ENTRY(_SC_NPROCESSORS_ONLN),
	ENTRY(_SC_PHYS_PAGES),
	ENTRY(_SC_AVPHYS_PAGES),
	ENTRY(_SC_ATEXIT_MAX),
	ENTRY(_SC_PASS_MAX),
	ENTRY(_SC_XOPEN_VERSION),
	ENTRY(_SC_XOPEN_XCU_VERSION),
	ENTRY(_SC_XOPEN_UNIX),
	ENTRY(_SC_XOPEN_CRYPT),
	ENTRY(_SC_XOPEN_ENH_I18N),
	ENTRY(_SC_XOPEN_SHM),
	ENTRY(_SC_2_CHAR_TERM),
	ENTRY(_SC_2_C_VERSION),
	ENTRY(_SC_2_UPE),
	ENTRY(_SC_XOPEN_XPG2),
	ENTRY(_SC_XOPEN_XPG3),
	ENTRY(_SC_XOPEN_XPG4),
	ENTRY(_SC_CHAR_BIT),
	ENTRY(_SC_CHAR_MAX),
	ENTRY(_SC_CHAR_MIN),
	ENTRY(_SC_INT_MAX),
	ENTRY(_SC_INT_MIN),
	ENTRY(_SC_LONG_BIT),
	ENTRY(_SC_WORD_BIT),
	ENTRY(_SC_MB_LEN_MAX),
	ENTRY(_SC_NZERO),
	ENTRY(_SC_SSIZE_MAX),
	ENTRY(_SC_SCHAR_MAX),
	ENTRY(_SC_SCHAR_MIN),
	ENTRY(_SC_SHRT_MAX),
	ENTRY(_SC_SHRT_MIN),
	ENTRY(_SC_UCHAR_MAX),
	ENTRY(_SC_UINT_MAX),
	ENTRY(_SC_ULONG_MAX),
	ENTRY(_SC_USHRT_MAX),
	ENTRY(_SC_NL_ARGMAX),
	ENTRY(_SC_NL_LANGMAX),
	ENTRY(_SC_NL_MSGMAX),
	ENTRY(_SC_NL_NMAX),
	ENTRY(_SC_NL_SETMAX),
	ENTRY(_SC_NL_TEXTMAX),
	ENTRY(_SC_XBS5_ILP32_OFF32),
	ENTRY(_SC_XBS5_ILP32_OFFBIG),
	ENTRY(_SC_XBS5_LP64_OFF64),
	ENTRY(_SC_XBS5_LPBIG_OFFBIG),
	ENTRY(_SC_XOPEN_LEGACY),
	ENTRY(_SC_XOPEN_REALTIME),
	ENTRY(_SC_XOPEN_REALTIME_THREADS),
	ENTRY(_SC_ADVISORY_INFO),
	ENTRY(_SC_BARRIERS),
	ENTRY(_SC_BASE),
	ENTRY(_SC_C_LANG_SUPPORT),
	ENTRY(_SC_C_LANG_SUPPORT_R),
	ENTRY(_SC_CLOCK_SELECTION),
	ENTRY(_SC_CPUTIME),
	ENTRY(_SC_THREAD_CPUTIME),
	ENTRY(_SC_DEVICE_IO),
	ENTRY(_SC_DEVICE_SPECIFIC),
	ENTRY(_SC_DEVICE_SPECIFIC_R),
	ENTRY(_SC_FD_MGMT),
	ENTRY(_SC_FIFO),
	ENTRY(_SC_PIPE),
	ENTRY(_SC_FILE_ATTRIBUTES),
	ENTRY(_SC_FILE_LOCKING),
	ENTRY(_SC_FILE_SYSTEM),
	ENTRY(_SC_MONOTONIC_CLOCK),
	ENTRY(_SC_MULTI_PROCESS),
	ENTRY(_SC_SINGLE_PROCESS),
	ENTRY(_SC_NETWORKING),
	ENTRY(_SC_READER_WRITER_LOCKS),
	ENTRY(_SC_SPIN_LOCKS),
	ENTRY(_SC_REGEXP),
	ENTRY(_SC_REGEX_VERSION),
	ENTRY(_SC_SHELL),
	ENTRY(_SC_SIGNALS),
	ENTRY(_SC_SPAWN),
	ENTRY(_SC_SPORADIC_SERVER),
	ENTRY(_SC_THREAD_SPORADIC_SERVER),
	ENTRY(_SC_SYSTEM_DATABASE),
	ENTRY(_SC_SYSTEM_DATABASE_R),
	ENTRY(_SC_TIMEOUTS),
	ENTRY(_SC_TYPED_MEMORY_OBJECTS),
	ENTRY(_SC_USER_GROUPS),
	ENTRY(_SC_USER_GROUPS_R),
	ENTRY(_SC_2_PBS),
	ENTRY(_SC_2_PBS_ACCOUNTING),
	ENTRY(_SC_2_PBS_LOCATE),
	ENTRY(_SC_2_PBS_MESSAGE),
	ENTRY(_SC_2_PBS_TRACK),
	ENTRY(_SC_SYMLOOP_MAX),
	ENTRY(_SC_STREAMS),
	ENTRY(_SC_2_PBS_CHECKPOINT),
	ENTRY(_SC_V6_ILP32_OFF32),
	ENTRY(_SC_V6_ILP32_OFFBIG),
	ENTRY(_SC_V6_LP64_OFF64),
	ENTRY(_SC_V6_LPBIG_OFFBIG),
	ENTRY(_SC_HOST_NAME_MAX),
	ENTRY(_SC_TRACE),
	ENTRY(_SC_TRACE_EVENT_FILTER),
	ENTRY(_SC_TRACE_INHERIT),
	ENTRY(_SC_TRACE_LOG),
	ENTRY(_SC_LEVEL1_ICACHE_SIZE),
	ENTRY(_SC_LEVEL1_ICACHE_ASSOC),
	ENTRY(_SC_LEVEL1_ICACHE_LINESIZE),
	ENTRY(_SC_LEVEL1_DCACHE_SIZE),
	ENTRY(_SC_LEVEL1_DCACHE_ASSOC),
	ENTRY(_SC_LEVEL1_DCACHE_LINESIZE),
	ENTRY(_SC_LEVEL2_CACHE_SIZE),
	ENTRY(_SC_LEVEL2_CACHE_ASSOC),
	ENTRY(_SC_LEVEL2_CACHE_LINESIZE),
	ENTRY(_SC_LEVEL3_CACHE_SIZE),
	ENTRY(_SC_LEVEL3_CACHE_ASSOC),
	ENTRY(_SC_LEVEL3_CACHE_LINESIZE),
	ENTRY(_SC_LEVEL4_CACHE_SIZE),
	ENTRY(_SC_LEVEL4_CACHE_ASSOC),
	ENTRY(_SC_LEVEL4_CACHE_LINESIZE),
	ENTRY(_SC_IPV6),
	ENTRY(_SC_RAW_SOCKETS),
	ENTRY(_SC_V7_ILP32_OFF32),
	ENTRY(_SC_V7_ILP32_OFFBIG),
	ENTRY(_SC_V7_LP64_OFF64),
	ENTRY(_SC_V7_LPBIG_OFFBIG),
	ENTRY(_SC_SS_REPL_MAX),
	ENTRY(_SC_TRACE_EVENT_NAME_MAX),
	ENTRY(_SC_TRACE_NAME_MAX),
	ENTRY(_SC_TRACE_SYS_MAX),
	ENTRY(_SC_TRACE_USER_EVENT_MAX),
	ENTRY(_SC_XOPEN_STREAMS),
	ENTRY(_SC_THREAD_ROBUST_PRIO_INHERIT),
	ENTRY(_SC_THREAD_ROBUST_PRIO_PROTECT),
	ENTRY(_SC_MINSIGSTKSZ),
	ENTRY(_SC_SIGSTKSZ),
};

int main(int argc, char *argv[])
{
	struct conf_entry *e;
	long val;
	int i;

	if (argc == 1) {
		for (e = ARRAY_BEGIN(conf); e < ARRAY_END(conf); e++) {
			errno = 0;
			val = sysconf(e->number);
			if (errno)
				continue;

			log("%-26s %4d = %ld\n", e->name, e->number, val);
		}

		return 0;
	}

	if (argv[1][0] == '-' && argv[1][1] == 'b') {
		for (i = 0; i < 64 * 1024; i++) {
			errno = 0;
			val = sysconf(i);
			if (errno)
				continue;

			log("%05d = %ld\n", i, val);
		}

		return 0;
	}

	return 1;
}