CC=gcc

UNAME		:= $(shell uname -s)

src = $(wildcard *.c)
obj = $(src:.c=.o)

all: main

main:
	$(CC) -o dumpassort main.c dirutils.c reader.c diropt.c

debug:
	$(CC) -g -o dumpassort main.c dirutils.c reader.c diropt.c

.PHONY: clean
clean:
	rm -f $(obj) dumpassort
