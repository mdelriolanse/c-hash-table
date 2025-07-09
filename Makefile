CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -g

.PHONY: all test clean

all: test

test: ht.c ht.h main.c
	$(CC) $(CFLAGS) -o test ht.c main.c
	./test

clean:
	rm -f test
