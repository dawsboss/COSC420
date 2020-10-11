main: FactOverFlowBigInt.o BigInt.o
	mpicc -o main FactOverFlowBigInt.o BigInt.o -g -lm
BigInt.o: BigInt.h BigInt.c
	gcc -c BigInt.c -g
FactOverFlowBigInt.o: FactOverFlowBigInt.c
	mpicc -c FactOverFlowBigInt.c -g

clean:
	rm -f *.o main
