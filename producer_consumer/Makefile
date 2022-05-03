TARGET= candykids
OBJS = candykids.o bbuff.o stats.o candy_t.o candy_factory.o kid.o
CFLAGS = -Wall -g -std=c99 -Werror -D_POSIX_C_SOURCE=199309L
BINDIR = bin

.PHONY: clean test

vpath %.c src
vpath %.h headers
vpath %.o bin

CC_C = gcc

all: $(TARGET)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(BINDIR)/$@

$(TARGET): $(OBJS)
	$(CC_C) $(CFLAGS) $(BINDIR)/*.o -o $@ -lpthread -lrt

$(OBJS): $(BINDIR)

$(BINDIR):
	mkdir bin

test: all
	valgrind --leak-check=full --show-leak-kinds=all --num-callers=20 ./candykids $(TESTARGS)

clean:
	rm -rf $(TARGET) $(BINDIR)
