main: main.o citetree.o
	mpicc -o main main.o citetree.o -g
main.o: main.c citetree.h
	mpicc -c main.c -g
citetree.o: citetree.h citetree.c
	mpicc -c citetree.c -g
clean:
	rm -f *.o main
