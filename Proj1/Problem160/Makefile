main: Template.o
	mpicc -o main Template.o -g -lm
Template.o: Template.c
	mpicc -c Template.c -g

clean:
	rm -f *.o main
