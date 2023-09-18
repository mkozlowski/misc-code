MAKEFLAGS += -rR

ifeq ("$(origin CC)", "default")
    undefine CC
endif

CC ?= gcc

CFLAGS = -std=c11 -Wall -Wmissing-prototypes -O0 -g -Werror
LDFLAGS =

SRC = $(wildcard *.c)
EXE = $(SRC:%.c=%)
TXT = cc-version.txt cc-defines.txt

all: $(EXE) $(TXT)

$(EXE): %: %.c Makefile
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

cc-version.txt: Makefile
	echo CC=$(CC) > $@
	$(CC) --version >> $@
	$(CC) -v 2>> $@

cc-defines.txt:
	$(CC) $(CFLAGS) -dM -E - < /dev/null > $@

clean:
	rm -f $(EXE) $(TXT)
