writematrix: writematrix.o citetree.o
	mpicc writematrix.o citetree.o -o writematrix -g
writematrix.o: writematrix.c citetree.h
	mpicc -c writematrix.c -g
citetree.o: citetree.h citetree.c
	mpicc -c citetree.c -g
clean:
	rm -f *.o writematrix
