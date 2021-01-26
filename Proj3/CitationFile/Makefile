writematrix: writematrix.o citetree.o maxsparse
	mpicc writematrix.o citetree.o -o writematrix -g
maxsparse: maxsparse.o citetree.o
	mpicc maxsparse.o citetree.o -o maxsparse -g
maxsparse.o: maxsparse.c citetree.h
	mpicc -c maxsparse.c -g
writematrix.o: writematrix.c citetree.h
	mpicc -c writematrix.c -g
citetree.o: citetree.h citetree.c
	mpicc -c citetree.c -g
clean:
	rm -f *.o writematrix maxsparse
