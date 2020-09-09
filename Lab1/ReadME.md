### Ryan Rosiak and Grant Dawson
COSC 420-001
9/7/20

# Lab-1: Par
* Develop familiarity with C programming language
* Develop familiarity with parallel computing tool OpenMPI
* Explore empirical tests for program efficiency in terms of parallel computation and resources
 

## Lab Description:
This lab demonstrates beginner knowledge of the MPI (Message Passing Interface) library. The
first program is a hello world program written in C that is used to get our feet wet in the C
language as well as understand what it means for MPI to create multiple nodes and run various
copies of the same instruction. The Primal program uses the brute force method to figure out
if a number is prime or composite. The twist is that we use MPI to divide up the amount of
numbers that need to be checked. To do this, we look at the amount of numbers that need to be 
checked based off the given number input by the user. We only had to check up sqrt(N). Therefore,
depending on the amount of nodes specified by the user, we divided the sqrt(N) jobs by the amount
of nodes available and that will be the amount of numbers that each node will check. Overall, the
programs written show how the basics of parallel computing is implemented and how the C language 
interacts with the MPI Library.

Files to pay attention to:

Primal, hello - Output files that are run when the programs are executed

Makefile - File that allows the user to compile without having to input specific compilation steps

Makefile Instructions:
There are multiple options for making and running these two programs.

1. make hello
    * This command allows you to compile the hello.c file. **You must run the hello output file
    manually. Once 'make hello' is run, proceed with 'mpiexec -n (# of nodes) ./hello

2. make Primal
    * This command allows you to compile the Primal.c file. **You must run the Primal output file
    manually with this option. Once 'make Primal' is run, proceed with 
    'mpiexec -n (# of nodes) ./Primal (Num to check)

3. make run arg1="# of nodes" arg2="# to check"
    * This command will compile both Primal. and hello.c BUT WILL ONLY EXECUTE Primal output file
    with specific commands. There is no make run option for hello. arg1= the number of nodes
    you would like to have in your program and arg2= to the number that would you like to check for
    primality. **For both arguments, do not use quotes as seen above! **Running only 'make' will
    run the make run option so arguments must be specified for program to run in this way.

Instructions for Program (manual):
Follow the Makefile instructions above to run the program without any manual work. If not using
Makefile, then you must compile the .c files using mpicc (mpicc hello.c) or (mpicc Primal.c -lm).
After this, each program can be run as such, (mpiexec -n (# of nodes) ./(output file) (args...))
**You must have a working library of openmpi3 or mpich that is already in your PATH

## Questions:




#### (a) What is the theoretical time complexity of your sorting algorithms (best and worst case), in terms of the input size?
Best case O(1): This is the case when we can determine if the number is prime by what the first number is.
   This will occur with out program when the number is divisible by 2. 
   
Worst case O(sqrt(N) / p): p being the number of nodes and N is the given number. This is the case when
   the number is prime / when there are no factorsd and we had to ruin through ever number from 2 to sqrt(N). 
   And since we had p number of nodes each only had to do sqrt(N)/p number of numbers at the same time.

#### (b) According to the data, does adding more nodes perfectly divide the time taken by the program?

#### (c) Justify/prove the fact that one needs only check up to √N in the brute force primality test.
Note: sqrt(N)*sqrt(N) = N, we know that the sqrt(N) is the half way point of factors. 
   Let x,y be two factors of N
   Assume that y > sqrt(N), y=(N/x), N/x > sqrt(N) => sqrt(N) > x
   Which means x is smaller that sqrt(N) and smaller than y when y is > sqrt(N). Since x was samller than sqrt(N) we know
   that it was checked in the first "half" of the sqrt(N) checking so checking past sqrt(N) would be overkill and just 
   rechecking already checked factors. 

#### (d) How could the code be improved in terms of usability, efficiency, and robustness?
The code could be improved based in terms of usability by aggregating the data that each node
    displays. The best way to do this would be to aggregate each nodes finding in order from 0 to 
    sqrt(N). This would allow you to easily see the factors in order of N. The only other way that
    could make this program much better would be to implement a faster algorithm to determine 
    primality. This could be done by using another algorithm such as the Fermat primality test or
    the Miller-Rabin which use the properties of odd factors to determing if a number is prime without
    having to check every number up to sqrt(N).
