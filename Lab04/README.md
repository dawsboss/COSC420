### Ryan Rosiak and Grant Dawson
COSC 420-001
10/17/20

# Lab-4: Eigen Value Decomposition and Partial HITS Algorithm Implementation
In this lab you will focus on the following objectives:
1. Develop familiarity with C programming language
2. Develop familiarity with parallel computing tools MPICH and OpenMPI
3. Develop familiarity with linear algebra and solving linear systems of equations.
4. Explore empirical tests for program efficiency in terms of parallel computation and resources
 

## Lab Description:
This lab demonstrates parallel eigenvalue decomposition functions. This function
is done in two ways. One way using normal MPI techniques including various 
MPI collectives and pooling. The other way using MPI file shared access
and shared memory. Both functions are analyzed for speed compared to one 
another.

## Files to pay attention to:

driver, main, filetest  - Output files that are run when the programs are executed

Makefile - File that allows the user to compile without having to input specific commands

## How to Run:

1. make main
        - This command compiles the main.c file. This file outputs specific test cases for each
        matrix function to show correct functionality

2. make driver
        - This command compiles the driver.c file. While using the time shell function, this 
        file outputs the amount of time that each matrix function will take on various test cases

3. make filetest
        - This command compiles the filtest.c file. This gives an example of file writing and reading
        using the given infile and outfile

3. make
        - This will compile all files, allowing you to run at your leisure

## Instructions for Program (manual):
Follow the Makefile instructions above to run the program without any manual work. If not
using Makefile, then you must compil the .c files using mpicc (mpicc main.c). After this,
each program can be run as such, (mpiexec -n (# of nodes) ./(output file)
*You must have a working library of openmpi3 or mpich that is already in your PATH


***Note*** - There is a small issue that does not affect the code integrity. But Valgrind says there is a
               conditional jump at the beginning of the driver program. We have done extensive testing and
               analysis of this problem, and we determined that the issue is not ours and that MPI is at 
               fault. We have had issues like this before with MPI, and this only happens on one node 
               (no matter worldSize) and in the same spot. The code still runs and still performs its 
               operation. So we don't see this as a problem in the end.

## Results: 

#### Eigen Vector:

This graph makes sense in that as the matrix sizes increase, the amount of time
is increasing. However, if you look closer, you will notice that the number of
nodes line is jumping over one another and is spiking. This can be explained 
because the matrices are generated randomly, leading to issues where some 
matrices will not have eigenvectors and will take 10,000 iterations. Also, that
specific matrix size could have had hard to acceptable/matrices with no 
eigenvectors. 

![Eigen Vector](https://github.com/spa542/COSC420/blob/master/Lab4/Img/Eigen%20Vector.png)

#### Eigen Vector File:
Some explanation behind the strange looking graph below. One of the big notes
is that there is a lot of overhead that is introduced with the functions because
of files dirty business that we had to perform for the lab requirements.
There was a process of writing a matrix to a file, reading ti, and then writing
that output to a file to read into the main. This can be extremely 
annoying and takes a lot of fo time. The next issue that will arise is that 
the randomly generated matrix would not have an eigenvector. That would mean 
that the while loop would run the 10,000 times that we hardcoded in the 
function. But if you look closer, you will notice that it seems like the 
number of nodes line are jumping over one another and is spiking. This can
be explained because the matrices are generated randomly, and that leads to
issues where some matrices won't have eigenvectors and will take the 10,000 
iterations. Also, that specific matrix size could have had matrices that had
hard to acceptable/matrices with no eigenvectors. 

![Eigen Vector File](https://github.com/spa542/COSC420/blob/master/Lab4/Img/Eigen%20Vector%20File.png)


## Questions:

#### (a) What is the theoretical time complexity of your algorithms (best and worst case), in terms of the input size?
For the non file version of eigenvalue, the time complexity is O(c*2n^3 + (n/p)^2 + 2(n/p)). There 
is a matrix multiplication up to 10000 times making the first part 
10000*O(2n^3 + (n/p)^2). There is a normalization which takes O(n/p) and then a 
norm calculation which takes O(n/p).
For the file version of eigenvalue, the time complexity is O((n/p)^3 + 2*p*n + 4*(n/p)).
This is because we have each node doing serial versions of each calculation. There is a
n/p elements being multiplied using the serial multiplication function. Each node has
n/p rows, meaning that they will each compute a fraction of the l2norm in serial, so there
are n elements being multiplied p times. Lastly, there is the normalization in serial which
is n/p. 
Overall, both functions are unique. The mpi file eigenvector function is a lot slower 
despite the big O being a little bit better. This is because of all of the overhead. The main
loop can write up to 10000 times. Since there are no collectives, we have the added 
overhead of opening up a file to write all of the values to it and then open up the file
again to read all of the files to it. When looking at large test cases, this can be 
extremely costly. The version that uses collectives has a little larger big O, but cleans
up with much less overhead because there are only collective communications that need to 
be taken into account. Overall, they both have their uses, but a function with variety 
of both could beat the functions that stick strictly to one type of method.


#### (b) According to the data, does adding more nodes perfectly divide the time taken by the program?
No. Generally, there is a lot of overhead that comes with using MPI. The nodes communication creates a 
uncontrolled overhead. Along with this, the entire functions that are written can't be parallelized. There
are operations that are being used that are parallelized but the function running altogether leaves 
other processors unused. Also, with the version that uses files, the overhead is even greater because there are
multiple opening, closing, reading, and writing of files on every iteration. This is a lot of moving parts that 
can cause different possibilities when running. The timing varies from function to function and from different amount
of processors. Overall, the time can vary depening on those parameters.

#### (c) What are some real-world software examples that would need the above routines? Why? Would they benefit greatly from using your distributed code?  
The real world software examples that use these functions are algorithms like pagerank and the hits algorithm. They use these algorithms
to perform some linear algebra manipulations of the matrices that they give it in order to extract specific data that can be used to their
benefit. Just about any company that uses matrices to store data, any search engine such as yahoo, google, or bing need to have some way to decide
what they are going to give people when they search things up. These functions can be used to aid that process.


#### (d) How could the code be improved in terms of usability, efficiency, and robustness?
For both functions, the best way to combine these functions would be to use collectives and 
file reading and writing. This could be done by using collectives to pass data around (to reduce overhead),
and then let the matrices and vectors be read into from files from the start. This would allow us to get data
fast while reducing overhead when communicating. Another way to increase efficiency would be to find a way to 
split the rows of the matrix A like in the file eigen vector function, but run the multiplication of matrices and the l2norm 
in parallel as well. There would have to be a careful organization of nodes because as of right now, only one or the other method
is used in each function. Combining this in some way would create the best optimizations for these functions.
