TARGET = kallocation
OBJS = main.o kallocator.o mem_block.o

CFLAGS = -Wall -g -std=c99 -D_POSIX_C_SOURCE=199309L
CC = gcc

all: clean $(TARGET)

%.o : %.c
	$(CC) -c $(CFLAGS) $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

test: all
	valgrind --leak-check=full ./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f $(OBJS)
