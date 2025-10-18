all: parallel_min_max process_memory parallel_sum

parallel_min_max:
	-make -C ../../lab3/src parallel_min_max

process_memory: process_memory.c
	gcc -o process_memory process_memory.c -I. -w

parallel_sum: parallel_sum.c sum.c sum.h
	gcc -c sum.c -o sum.o -I. -w
	ar rcs libsum.a sum.o
	gcc -c parallel_sum.c -o parallel_sum.o -I. -w
	gcc -o parallel_sum parallel_sum.o -L. libsum.a -lpthread

clean:
	-make -C ../../lab3/src clean
	-rm process_memory
	-rm parallel_sum
	-rm *.o
	-rm libsum.a