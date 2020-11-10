main: main.o matrix.o driver filetest
	mpicc -o main main.o matrix.o -g -lm
filetest: filetest.o matrix.o
	mpicc -o filetest filetest.o matrix.o -g -lm
driver: driver.o matrix.o
	mpicc -o driver driver.o matrix.o -g -lm
main.o: main.c matrix.h -lm
	mpicc -c main.c -g -lm
filetest.o: filetest.c matrix.h
	mpicc -c filetest.c -g -lm
driver.o: driver.c matrix.h
	mpicc -c driver.c -g -lm	
matrix.o: matrix.c matrix.h
	mpicc -c matrix.c -g -lm -ldl
clean:
	rm -f *.o main driver filetest
