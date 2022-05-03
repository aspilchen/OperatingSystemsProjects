TARGET := process_ancestors_test
CC := gcc
CFLAGS = -Wall -g -std=c99 -D _GNU_SOURCE -static -pthread -Werror

all: clean $(TARGET) export

$(TARGET):
	$(CC) $(CFLAGS) $@.c -o $@

.PHONY: export
export:
	scp $(TARGET) qemu:~ 

.PHONY: clean
clean:
	rm -f $(TARGET)
