## This is a simple Makefile with lost of comments 
## Check Unix Programming Tools handout for more info.

# Define what compiler to use and the flags.
CC=cc
CXX=CC
CFLAGS= -c -g -std=c99 -Wall -Werror
.PHONY: clean

all: test_list test

test_list: list.o test_list.o test_functions.o

test: test_list
	./test_list

clean: 
	rm -f *.o test_list

%.o: %.c
	$(CC) $(CFLAGS) $<
