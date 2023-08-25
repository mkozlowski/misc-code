#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <complex.h>

#define SIZE_OF(t) \
	printf("%-22s%zu byt%s\n", #t, sizeof(t), sizeof(t) > 1 ? "es" : "e")

#define NO_TYPE(t) \
	printf("%-22snot available\n", #t)

int main(void)
{
	SIZE_OF(void *);
	SIZE_OF(char);
	SIZE_OF(int);
	SIZE_OF(long);
	SIZE_OF(long long);
	SIZE_OF(short);
	SIZE_OF(unsigned);
	SIZE_OF(float);
	SIZE_OF(double);
	SIZE_OF(long double);
	SIZE_OF(_Bool);
	SIZE_OF(_Complex float);
	SIZE_OF(_Complex double);
	SIZE_OF(_Complex long double);
#ifdef __SIZEOF_INT128__
	SIZE_OF(__int128);
#else
	NO_TYPE(__int128);
#endif

	return 0;
}
