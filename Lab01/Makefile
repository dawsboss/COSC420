run: Primal hello
	mpiexec -n $(arg1) ./Primal $(arg2)
Primal: Primal.c
	mpicc Primal.c -lm -o Primal
hello: hello.c
	mpicc hello.c -o hello
clean:
	rm -f *.o hello Primal
