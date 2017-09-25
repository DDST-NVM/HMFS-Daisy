CC=gcc
CFLAGS=-I.

all:
	$(CC) -m64 -c benchmark.c -o benchmark.o
	$(CC) -m64 -c redis-bench.c -o redis-bench.o
	$(CC) -m64 -c p_mmap.c -o p_mmap.o
	$(CC) -m64 -static -g -o benchmark benchmark.o p_mmap.o -I.
	$(CC) -m64 -static -g -o redis-bench redis-bench.o p_mmap.o -I.
clean:
	@rm -rf *.o *.txt benchmark redis-bench 
