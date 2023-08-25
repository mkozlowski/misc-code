#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "common.h"

/*
 * See kernel ./include/linux/errno.h.
 * These should never be seen by user programs.
 */
#define ERESTARTSYS		512
#define ERESTARTNOINTR		513
#define ERESTARTNOHAND		514
#define ENOIOCTLCMD		515
#define ERESTART_RESTARTBLOCK	516
#define EPROBE_DEFER		517
#define EOPENSTALE		518
#define ENOPARAM		519
/* NFSv3 protocol */
#define EBADHANDLE		521
#define ENOTSYNC		522
#define EBADCOOKIE		523
#define ENOTSUPP		524
#define ETOOSMALL		525
#define ESERVERFAULT		526
#define EBADTYPE		527
#define EJUKEBOX		528
#define EIOCBQUEUED		529
#define ERECALLCONFLICT		530
#define ENOGRACE		531

#define ENTRY(e) { (e), #e }

struct errno_entry {
	int number;
	char *name;
} static err[] = {
	ENTRY(EPERM),
	ENTRY(ENOENT),
	ENTRY(ESRCH),
	ENTRY(EINTR),
	ENTRY(EIO),
	ENTRY(ENXIO),
	ENTRY(E2BIG),
	ENTRY(ENOEXEC),
	ENTRY(EBADF),
	ENTRY(ECHILD),
	ENTRY(EAGAIN),
	ENTRY(ENOMEM),
	ENTRY(EACCES),
	ENTRY(EFAULT),
	ENTRY(ENOTBLK),
	ENTRY(EBUSY),
	ENTRY(EEXIST),
	ENTRY(EXDEV),
	ENTRY(ENODEV),
	ENTRY(ENOTDIR),
	ENTRY(EISDIR),
	ENTRY(EINVAL),
	ENTRY(ENFILE),
	ENTRY(EMFILE),
	ENTRY(ENOTTY),
	ENTRY(ETXTBSY),
	ENTRY(EFBIG),
	ENTRY(ENOSPC),
	ENTRY(ESPIPE),
	ENTRY(EROFS),
	ENTRY(EMLINK),
	ENTRY(EPIPE),
	ENTRY(EDOM),
	ENTRY(ERANGE),
	ENTRY(EDEADLK),
	ENTRY(ENAMETOOLONG),
	ENTRY(ENOLCK),
	ENTRY(ENOSYS),
	ENTRY(ENOTEMPTY),
	ENTRY(ELOOP),
	ENTRY(EWOULDBLOCK),
	ENTRY(ENOMSG),
	ENTRY(EIDRM),
	ENTRY(ECHRNG),
	ENTRY(EL2NSYNC),
	ENTRY(EL3HLT),
	ENTRY(EL3RST),
	ENTRY(ELNRNG),
	ENTRY(EUNATCH),
	ENTRY(ENOCSI),
	ENTRY(EL2HLT),
	ENTRY(EBADE),
	ENTRY(EBADR),
	ENTRY(EXFULL),
	ENTRY(ENOANO),
	ENTRY(EBADRQC),
	ENTRY(EBADSLT),
	ENTRY(EDEADLOCK),
	ENTRY(EBFONT),
	ENTRY(ENOSTR),
	ENTRY(ENODATA),
	ENTRY(ETIME),
	ENTRY(ENOSR),
	ENTRY(ENONET),
	ENTRY(ENOPKG),
	ENTRY(EREMOTE),
	ENTRY(ENOLINK),
	ENTRY(EADV),
	ENTRY(ESRMNT),
	ENTRY(ECOMM),
	ENTRY(EPROTO),
	ENTRY(EMULTIHOP),
	ENTRY(EDOTDOT),
	ENTRY(EBADMSG),
	ENTRY(EOVERFLOW),
	ENTRY(ENOTUNIQ),
	ENTRY(EBADFD),
	ENTRY(EREMCHG),
	ENTRY(ELIBACC),
	ENTRY(ELIBBAD),
	ENTRY(ELIBSCN),
	ENTRY(ELIBMAX),
	ENTRY(ELIBEXEC),
	ENTRY(EILSEQ),
	ENTRY(ERESTART),
	ENTRY(ESTRPIPE),
	ENTRY(EUSERS),
	ENTRY(ENOTSOCK),
	ENTRY(EDESTADDRREQ),
	ENTRY(EMSGSIZE),
	ENTRY(EPROTOTYPE),
	ENTRY(ENOPROTOOPT),
	ENTRY(EPROTONOSUPPORT),
	ENTRY(ESOCKTNOSUPPORT),
	ENTRY(EOPNOTSUPP),
	ENTRY(EPFNOSUPPORT),
	ENTRY(EAFNOSUPPORT),
	ENTRY(EADDRINUSE),
	ENTRY(EADDRNOTAVAIL),
	ENTRY(ENETDOWN),
	ENTRY(ENETUNREACH),
	ENTRY(ENETRESET),
	ENTRY(ECONNABORTED),
	ENTRY(ECONNRESET),
	ENTRY(ENOBUFS),
	ENTRY(EISCONN),
	ENTRY(ENOTCONN),
	ENTRY(ESHUTDOWN),
	ENTRY(ETOOMANYREFS),
	ENTRY(ETIMEDOUT),
	ENTRY(ECONNREFUSED),
	ENTRY(EHOSTDOWN),
	ENTRY(EHOSTUNREACH),
	ENTRY(EALREADY),
	ENTRY(EINPROGRESS),
	ENTRY(ESTALE),
	ENTRY(EUCLEAN),
	ENTRY(ENOTNAM),
	ENTRY(ENAVAIL),
	ENTRY(EISNAM),
	ENTRY(EREMOTEIO),
	ENTRY(EDQUOT),
	ENTRY(ENOMEDIUM),
	ENTRY(EMEDIUMTYPE),
	ENTRY(ECANCELED),
	ENTRY(ENOKEY),
	ENTRY(EKEYEXPIRED),
	ENTRY(EKEYREVOKED),
	ENTRY(EKEYREJECTED),
	ENTRY(EOWNERDEAD),
	ENTRY(ENOTRECOVERABLE),
	ENTRY(ERFKILL),
	ENTRY(EHWPOISON),
	/* ---- */
	ENTRY(ERESTARTSYS),
	ENTRY(ERESTARTNOINTR),
	ENTRY(ERESTARTNOHAND),
	ENTRY(ENOIOCTLCMD),
	ENTRY(ERESTART_RESTARTBLOCK),
	ENTRY(EPROBE_DEFER),
	ENTRY(EOPENSTALE),
	ENTRY(ENOPARAM),
	ENTRY(EBADHANDLE),
	ENTRY(ENOTSYNC),
	ENTRY(EBADCOOKIE),
	ENTRY(ENOTSUPP),
	ENTRY(ETOOSMALL),
	ENTRY(ESERVERFAULT),
	ENTRY(EBADTYPE),
	ENTRY(EJUKEBOX),
	ENTRY(EIOCBQUEUED),
	ENTRY(ERECALLCONFLICT),
	ENTRY(ENOGRACE),
};

static void print_errno(struct errno_entry *e)
{
	log("%-22s %-4d %s\n", e->name, e->number, strerror(e->number));
}

int main(int argc, char *argv[])
{
	char *endptr;
	long val;
	struct errno_entry *e;

	if (argc == 1) {
		for (e = ARRAY_BEGIN(err); e < ARRAY_END(err); e++)
			print_errno(e);

		return 0;
	}

	if (argv[1][0] == 'E') {
		for (e = ARRAY_BEGIN(err); e < ARRAY_END(err); e++)
			if (!strncmp(e->name, argv[1], strlen(argv[1])))
				print_errno(e);

		return 0;
	}

	val = strtol(argv[1], &endptr, 10);
	if (errno)
		die("strtol() failed: %m\n");

	if (endptr == argv[1])
		die("not a number: %s", argv[1]);

	for (e = ARRAY_BEGIN(err); e < ARRAY_END(err); e++) {
		if (e->number == val) {
			print_errno(e);
			return 0;
		}
	}

	die("errno %ld not found\n", val);
}