MAKEFLAGS += -rR

ifeq ("$(origin CC)", "default")
    undefine CC
endif

CC ?= $(CROSS_COMPILE)gcc
RC ?= rustc

CFLAGS = -std=c11 -Wall -Wmissing-prototypes -O0 -g -Werror
LDFLAGS =

SRC = $(wildcard *.c)
ALL = $(SRC:%.c=%)
ALT = vdso
DEF := $(filter-out $(ALT), $(ALL))
TXT = cc-version.txt cc-defines.txt

RS  = $(wildcard *.rs)
RSB = $(RS:%.rs=%)

default: $(DEF) $(ALT) $(TXT)
rust: $(RSB)

$(DEF): %: %.c Makefile
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(RSB): %: %.rs Makefile
	$(RC) $<

vdso: %: %.c Makefile
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS) -ldl

cc-version.txt: Makefile
	echo CC=$(CC) > $@
	$(CC) --version >> $@
	$(CC) -v 2>> $@

cc-defines.txt:
	$(CC) $(CFLAGS) -dM -E - < /dev/null > $@

clean:
	rm -f $(DEF) $(ALT) $(TXT) $(RSB)
