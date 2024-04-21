DMALLOCFLAGS = -DDMALLOC -DDMALLOC_FUNC_CHECK
DMALLOCLINK = -L/usr/local/lib/libdmalloc.a -ldmalloc

GPROFLAGS = -pg

CC = gcc
CFLAGS = -Wall -Wextra -ggdb
CLINKFLAGS = -lm

main.out: main.c
	$(CC) -o main.out $(CFLAGS) main.c $(CLINKFLAGS)

gprof: main.c
	$(CC) $(GPROFLAGS) -o main.out $(CFLAGS) main.c $(CLINKFLAGS)

dmalloc: main.c
	$(CC) $(DMALLOCFLAGS) -o main.out $(CFLAGS) main.c $(CLINKFLAGS) $(DMALLOCLINK)


test: test.c
	$(CC) $(DMALLOCFLAGS) -o test.out $(CFLAGS) test.c $(CLINKFLAGS) $(DMALLOCLINK)

clear:
	rm -rf *.out *.avl