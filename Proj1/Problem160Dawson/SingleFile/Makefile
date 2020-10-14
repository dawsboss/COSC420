main: BigIntegerSingleFile.o main.o
	./mpipp -o main main.o BigIntegerSingleFile.o 

main.o: main.c
	mpicc -c -g main.c
test: BigIntegerSingleFile.o test.o
	g++ -o test test.o BigIntegerSingleFile.o
test.o: test.c
	gcc -c -g test.c
BigIntegerSingleFile.o: BigIntegerSingleFile.cpp
	g++ -c -g BigIntegerSingleFile.cpp

clean: 
	rm *.o test main
