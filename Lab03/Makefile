main: main.o matrix.o driver filetest
	mpicc -o main main.o matrix.o -g
filetest: filetest.o matrix.o
	mpicc -o filetest filetest.o matrix.o -g
driver: driver.o matrix.o
	mpicc -o driver driver.o matrix.o -g
main.o: main.c matrix.h
	mpicc -c main.c -g
filetest.o: filetest.c matrix.h
	mpicc -c filetest.c -g
driver.o: driver.c matrix.h
	mpicc -c driver.c -g	
matrix.o: matrix.c matrix.h
	mpicc -c matrix.c -g -lm
clean:
	rm -f *.o main driver filetest
