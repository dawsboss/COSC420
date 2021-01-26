main: main.o matrix.o PageRank.o
	mpicc -o main main.o matrix.o PageRank.o -g -lm
main.o: main.c matrix.o -lm
	mpicc -c main.c -g -lm	
matrix.o: MatrixJawn/matrix.c MatrixJawn/matrix.h
	mpicc -c MatrixJawn/matrix.c -g -lm -ldl
PageRank.o: PageRank.c PageRank.h
	mpicc -c PageRank.c -g -lm

clean:
	rm -f *.o main
