CC = gcc
CFLAGS = -Wall -Wextra -ggdb


DMALLOCFLAGS = -DDMALLOC -DDMALLOC_FUNC_CHECK
DMALLOCLINK = -L/usr/local/lib/libdmalloc.a -ldmalloc

main.out: main.c
	$(CC) -o main.out $(CFLAGS) main.c -lm

dmalloc: main.c
	$(CC) $(DMALLOCFLAGS) -o main.out $(CFLAGS) main.c -lm $(DMALLOCLINK)

test: test.c
	$(CC) $(DMALLOCFLAGS) -o test.out $(CFLAGS) test.c -lm $(DMALLOCLINK)

clear:
	rm -rf *.out