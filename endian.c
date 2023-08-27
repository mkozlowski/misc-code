#include <stdio.h>

/*
 * Testing big endian arm code with qemu:
 *
 * Get cross compiler and matching sysroot, e.g. from Linaro page:
 * https://releases.linaro.org/components/toolchain/binaries/latest-7/aarch64_be-linux-gnu/
 *
 * gcc-linaro-7.5.0-2019.12-x86_64_aarch64_be-linux-gnu.tar.xz
 * sysroot-glibc-linaro-2.25-2019.12-aarch64_be-linux-gnu.tar.xz
 *
 * Install qemu-user
 * sudo apt install qemu-user
 *
 * Then
 *
 * aarch64_be-linux-gnu-gcc endian.c -o endian
 * qemu-aarch64_be -L /tmp/sysroot-glibc-linaro-2.25-2019.12-aarch64_be-linux-gnu/ ./endian
 *
 * or
 *
 * aarch64_be-linux-gnu-gcc endian.c -o endian -static
 * qemu-aarch64_be ./endian
 *
 */

int main(void)
{
	printf("code: ");

	if (*(char *)&(unsigned long){1} == 1)
		printf("little endian\n");
	else
		printf("big endian\n");

	printf("macro: ");

#if defined(__BYTE_ORDER__)
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		printf("little endian");
	#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		printf("big endian");
	#elif __BYTE_ORDER__ == __ORDER_PDP_ENDIAN__
		printf("PDP endian");
	#else
		printf("?");
	#endif

	printf(" (%d)\n", __BYTE_ORDER__);
#else
	printf("__BYTE_ORDER__ is not defined\n");
#endif

	return 0;
}
