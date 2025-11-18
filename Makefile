CC = gcc
CFLAGS = -Wall -Werror -std=c99 -g \
	-fno-builtin-malloc -fno-builtin-free \
	-fno-builtin-calloc -fno-builtin-realloc

all: test

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c -o mymalloc.o

test.o: test.c mymalloc.h
	$(CC) $(CFLAGS) -c test.c -o test.o

test: mymalloc.o test.o
	$(CC) $(CFLAGS) mymalloc.o test.o -o test

clean:
	rm -f test *.o

