TARGET = array_stats_test
CC = gcc
CFLAGS = -Wall -g -std=c99 -D _GNU_SOURCE -static -pthread -Werror

all: $(TARGET) export

$(TARGET):
	$(CC) $(CFLAGS) $@.c -o $@

.PHONY: clean
clean:
	rm -f $(TARGET)

.PHONY: export
export:
	scp $(TARGET) qemu:~
