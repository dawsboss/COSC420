main: main.o BigIntegerSingleFile.o
	./mpipp -o main main.o BigIntegerSingleFile.o -g -lm
main.o: main.c BigIntegerSingleFile.cpp
	mpicc -c main.c -g
BigIntegerSingleFile.o: BigIntegerSingleFile.cpp
	g++ -c BigIntegerSingleFile.cpp -g
clean:
	rm -f *.o main
