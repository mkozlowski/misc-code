MAKEFLAGS += -rR

ifeq ("$(origin CC)", "default")
    undefine CC
endif

CC ?= gcc

CFLAGS = -std=c11 -Wall -Wmissing-prototypes -O0 -g -Werror
LDFLAGS =

SRC = $(wildcard *.c)
EXE = $(SRC:%.c=%)

all: $(EXE)

$(EXE): %: %.c Makefile
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(EXE)
