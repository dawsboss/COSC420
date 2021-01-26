main: main.o matrix.o
	mpicc -o main matrix.o main.o -g -lm
main.o: main.c matrix.h
	mpicc -c main.c -g -lm
matrix.o: matrix.h matrix.c
	mpicc -c matrix.c -g -lm
clean:
	rm -f *.o main
