#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <sys/auxv.h>
#include <errno.h>

#include "common.h"

extern char **environ;

enum {
	DEC = 1,
	HEX,
	STR,
};

#define ENTRY(e, t, d) { (e), (#e), (t), (d) }

struct aux_entry {
	char number;
	char *name;
	char type;
	char *desc;
} static aux[] = {
	ENTRY(AT_NULL,              HEX, "End of vector"),
	ENTRY(AT_IGNORE,            HEX, "Entry should be ignored"),
	ENTRY(AT_EXECFD,            DEC, "File descriptor of program"),
	ENTRY(AT_PHDR,              HEX, "Program headers for program"),
	ENTRY(AT_PHENT,             DEC, "Size of program header entry"),
	ENTRY(AT_PHNUM,             DEC, "Number of program headers"),
	ENTRY(AT_PAGESZ,            DEC, "System page size"),
	ENTRY(AT_BASE,              HEX, "Base address of interpreter"),
	ENTRY(AT_FLAGS,             HEX, "Flags"),
	ENTRY(AT_ENTRY,             HEX, "Entry point of program"),
	ENTRY(AT_NOTELF,            HEX, "Program is not ELF"),
	ENTRY(AT_UID,               DEC, "Real uid"),
	ENTRY(AT_EUID,              DEC, "Effective uid"),
	ENTRY(AT_GID,               DEC, "Real gid"),
	ENTRY(AT_EGID,              DEC, "Effective gid"),
	ENTRY(AT_CLKTCK,            DEC, "Frequency of times()"),
	ENTRY(AT_PLATFORM,          STR, "String identifying platform"),
	ENTRY(AT_HWCAP,             HEX, "Machine-dependent hints about processor capabilities"),
	ENTRY(AT_FPUCW,             HEX, "Used FPU control word"),
	ENTRY(AT_DCACHEBSIZE,       DEC, "Data cache block size"),
	ENTRY(AT_ICACHEBSIZE,       DEC, "Instruction cache block size"),
	ENTRY(AT_UCACHEBSIZE,       DEC, "Unified cache block size"),
	ENTRY(AT_IGNOREPPC,         HEX, "Entry should be ignored"),
	ENTRY(AT_SECURE,            DEC, "Boolean, was exec setuid-like?"),
	ENTRY(AT_BASE_PLATFORM,     STR, "String identifying real platforms"),
	ENTRY(AT_RANDOM,            HEX, "Address of 16 random bytes"),
	ENTRY(AT_HWCAP2,            HEX, "More machine-dependent hints about processor capabilities"),
	ENTRY(AT_EXECFN,            STR, "Filename of executable"),
	ENTRY(AT_SYSINFO,           HEX, "The entry point to the system call function in the vDSO (absent on x86-64)"),
	ENTRY(AT_SYSINFO_EHDR,      HEX, "The address of a page containing the vDSO"),
	ENTRY(AT_L1I_CACHESHAPE,    HEX, "Shape of the L1 instruction cache"),
	ENTRY(AT_L1D_CACHESHAPE,    HEX, "Shape of the L1 data cache"),
	ENTRY(AT_L2_CACHESHAPE,     HEX, "Shape of the L2 cache"),
	ENTRY(AT_L3_CACHESHAPE,     HEX, "Shape of the L3 cache"),
	ENTRY(AT_L1I_CACHESIZE,     DEC, "The L1 instruction cache size"),
	ENTRY(AT_L1I_CACHEGEOMETRY, HEX, "Geometry of the L1 instruction cache"),
	ENTRY(AT_L1D_CACHESIZE,     DEC, "The L1 data cache size"),
	ENTRY(AT_L1D_CACHEGEOMETRY, HEX, "Geometry of the L1 data cache"),
	ENTRY(AT_L2_CACHESIZE,      DEC, "The L2 cache size"),
	ENTRY(AT_L2_CACHEGEOMETRY,  HEX, "Geometry of the L2 cache"),
	ENTRY(AT_L3_CACHESIZE,      DEC, "The L3 cache size"),
	ENTRY(AT_L3_CACHEGEOMETRY,  HEX, "Geometry of the L2 cache"),
#ifdef AT_MINSIGSTKSZ
	ENTRY(AT_MINSIGSTKSZ,       DEC, "Minimal stack size for signal delivery"),
#endif
};

int main(int argc, char *argv[])
{
	struct aux_entry *e;
	int i;
	int opt_aux = 0;
	int opt_env = 0;
	int opt_arg = 0;

	while ((i = getopt(argc, argv, "xea")) != -1) {
		switch (i) {
			case 'x':
				opt_aux = 1;
				break;
			case 'e':
				opt_env = 1;
				break;
			case 'a':
				opt_arg = 1;
				break;
			default:
				exit(1);
		}
	}

	if (!opt_aux && !opt_env && !opt_arg)
		opt_aux = opt_env = opt_arg = 1;

	for (e = ARRAY_BEGIN(aux); opt_aux && e < ARRAY_END(aux); e++) {
		unsigned long val;

		val = getauxval(e->number);
		if (!val && errno == ENOENT)
			continue;

		log("aux[%s]%*s= ", e->name, 21 - (int)strlen(e->name), " ");

		switch (e->type) {
			case DEC:
				log("%*lu", 2 * (int)sizeof(unsigned long), val);
				break;
			case HEX:
				log("%0*lx", 2 * (int)sizeof(unsigned long), val);
				break;
			case STR:
				log("%-*s", 2 * (int)sizeof(unsigned long), (char *)val);
				break;
			default:
				die("unhandled type %d\n", e->type);
		}

		log("  (%s)\n", e->desc);
	};

	for (i = 0; opt_env && environ[i]; i++)
		log("env[%d] = %s\n", i, environ[i]);

	for (i = 0; opt_arg && i < argc; i++)
		log("arg[%d] = %s\n", i, argv[i]);

	return 0;
}
