main: main.o bst.o
	gcc -o main main.o bst.o -g
main.o: main.c bst.h
	gcc -c main.c -g
bst.o: bst.h bst.c
	gcc -c bst.c -g
clean:
	rm -f *.o main
