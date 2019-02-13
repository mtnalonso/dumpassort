CC=gcc

UNAME		:= $(shell uname -s)
$(info Building for ${UNAME});

src = $(wildcard *.c)
obj = $(src:.c=.o)

all: main

main:
	$(CC) -o dumpassort main.c reader.c

debug:
	$(CC) -g -o dumpassort main.c reader.c

.PHONY: clean
clean:
	rm -f $(obj) dumpassort
