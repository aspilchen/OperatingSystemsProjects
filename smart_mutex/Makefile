TARGET = locking
OBJS = main.o klock.o thread_dll.o resource_table.o

CFLAGS = -Wall -g -std=c99 -Werror -pthread -lrt -D_POSIX_C_SOURCE=199309L
CC = gcc

.PHONY: clean test

all: clean $(TARGET)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

test: all
	valgrind --leak-check=full --show-leak-kinds=all --num-callers=20 ./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
