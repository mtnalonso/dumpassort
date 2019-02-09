CC=gcc

UNAME		:= $(shell uname -s)
$(info Building for ${UNAME});

all: generate

debug:
	gcc -g -o dumpassort main.c

generate:
	gcc main.c -o dumpassort
