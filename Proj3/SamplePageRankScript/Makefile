run: writeproutput.o
	mpicc -o run writeproutput.o -g
writeproutput.o: writeproutput.c
	mpicc -c writeproutput.c -g
clean:
	rm -f *.o run
