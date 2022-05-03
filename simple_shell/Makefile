## This is a simple Makefile with lots of comments 
## Check Unix Programming Tools handout for more info.

# Define what compiler to use and the flags.
CC=cc
CXX=CC
CCFLAGS= -g -std=c99 -D_POSIX_C_SOURCE=200809L -Wall -Werror
vpath %.c src
vpath %.h headers
.PHONY: clean all test

all: bin shell 

# Create binary directory
bin:
	mkdir $@

# Compile all .c files into .o files
%.o: %.c
	$(CC) -c $(CCFLAGS) $< -o bin/$@

# Link binary files
shell: shell.o built_in_commands.o shell_history.o interface_functions.o signal_handler.o
	$(CC) -o shell bin/*.o $(CCFLAGS)


clean:
	rm -rf bin shell
