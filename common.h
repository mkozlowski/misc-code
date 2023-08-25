#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define ARRAY_SIZE(a) (sizeof(a)/(sizeof(a[0])))
#define ARRAY_BEGIN(a) (&a[0])
#define ARRAY_END(a) (a + ARRAY_SIZE(a))

#define log(...) fprintf(stdout, __VA_ARGS__)
#define err(...) fprintf(stderr, __VA_ARGS__)
#define die(...) \
	do { \
		int tmp = errno; \
		fflush(stdout); \
		errno = tmp; \
		err(__VA_ARGS__); \
		exit(1); \
	} while (0)

#endif