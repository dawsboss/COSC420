### Ryan Rosiak and Grant Dawson
COSC 420-001
10/17/20

# Lab-3: Matrices Optomization and Gauss Jordan
In this lab you will focus on the following objectives:
1. Develop familiarity with C programming language
2. Develop familiarity with parallel computing tools MPICH and OpenMPI
3. Develop familiarity with linear algebra and solving linear systems of equations.
4. Explore empirical tests for program efficiency in terms of parallel computation and resources
 

## Lab Description:
This lab demonstrates parallel matrix functions. We implemented every basic
matrix operation such as addition, subtraction, multiplication, and inner 
product with the help of parallel computing. These functions go beyond
basic functionality by using MPICH to divide up our matrices during 
computation in order to make our functions faster.

## Files to pay attention to:

driver, main - Output files that are run when the programs are executed

Makefile - File that allows the user to compile without having to input specific commands

## How to Run:

1. make main
        - This command compiles the main.c file. This file outputs specific test cases for each
        matrix function to show correct functionality

2. make driver
        - This command compiles the driver.c file. While using the time shell function, this 
        file outputs the amount of time that each matrix function will take on various test cases

3. make
        - This will compile all files, allowing you to run at your leisure

## Instructions for Program (manual):
Follow the Makefile instructions above to run the program without any manual work. If not
using Makefile, then you must compil the .c files using mpicc (mpicc main.c). After this,
each program can be run as such, (mpiexec -n (# of nodes) ./(output file)
*You must have a working library of openmpi3 or mpich that is already in your PATH


## Results: 

The results were pulled from running the driver on numtiple world sizes/multiple number of nodes. 
The code was run and times 10 times and then the average was taken of those 10 times to get a "accurate"
time for a single run on a large matrix. It was very interesting to note that the times for one and two 
nodes were better than the five and ten on the smaller cases and as the cases got bigger it was better with more nodes.
This makes sense because the more node you have the larger the setup time for not that much more payout weith the extra computing time 
because then there is more cleaning up to do. 

Another interesting event that was noticed was there were some spikes in times. We aren't too sure where these came from other than 
the physical limiation of the machine running the tests. The tests are being run all at the same time on one PC. 

NOTE: that the Gauss Jordan also did not finish for the 1 node case. THis took over 24 hours to get what we had and due dates requested the graph as is. 


![Addtion Time](https://github.com/spa542/COSC420/blob/master/Lab3/Img/Addition.png)
![Subtraction Time](https://github.com/spa542/COSC420/blob/master/Lab3/Img/Subtraction.png)
![Inner Product Time](https://github.com/spa542/COSC420/blob/master/Lab3/Img/Inner%20Product.png)
![Multiplication Time](https://github.com/spa542/COSC420/blob/master/Lab3/Img/Multiplication.png)
![Gauss Jordan Time](https://github.com/spa542/COSC420/blob/master/Lab3/Img/GaussJordan.png)

## Questions:

#### (a) What is the theoretical time complexity of your algorithms (best and worst case), in terms of the input size?
        
        N being the total amount of elements in the array and P being the amount of processors in use.
        
        For the addition and subtraction, our algorithms are approximately O(N/P). This leads to a 
        best case of O(1), where every processor has one element and a worst case of O(N) where we
        have only one processor that must  do all of the work itself. 
        
        For the inner product, it is O(2N/P). For this, the best case is again O(1) because we 
        have N multiplications with N-1 additions and a reduced addition based off the amount of
        processors we have. The worst case is O(2N) because one processor will have to do all of 
        the additions and all of the multiplications by itself.
        
        For transpose is O(N). The best case is if we have a one element matrix which is O(1), and
        worst case is O(N). 
        
        For matrix multiplication is O((2N)^3 + (N/P)^2). This is because we have 2 copies
        of arrays N^3 times and the inner product is called N^2 times. This gives us a best case of
        O(N^3 + 1). This is if we only need to do the copies and we have enough processors for one
        node per processor. The worst case is O((2N)^3 + N^2). This is  if one processor has to do
        all of the work. 
        
        Finally for Gauss Jordan, the O(N*(N/p)^). This is because we iuterate on ever row of 'a' matrix.
        That is the 'k' loop where we scatter that pivviot row, b's piviot row, and the computation done
        using the kth row. Then we scatter all of a and all of b to their choosen nodes. Then O(N^2)
        proccess would be don on those if we only had one node but since we have p, it is reduced to O((N/p)^2)

#### (b) According to the data, does adding more nodes perfectly divide the time taken by the program?

        The program starts to go faster and faster. The timing is not perfectly divided by the amount
        of nodes that are being used purely because there is a lot of overhead that goes along with
        having a lot of nodes. Through the big O analysis, the best and worst cases show that there
        is a difference and the change over time is closely related with P. I believe that all of the
        functions see a natural P times faster based off the amount of nodes except for matrix multiplication
        and Gauss Jordan. This is because we implemented the function in such a way that only allows the
        processors to speed up certain parts of the function. Overall, the general consensus is that the
        more processors you  have, the faster the program will be. But the whole algorithm will not benifit
        from the increase of porcessing power. For exsample Gauss Jordan will still have to do the base 
        for loop where it labels a row as the kth pivot row. The inner two loops will be split up evenly but
        that outter loop can't be spead up with more proccessors all of it will still have to happen on all 
        the processors. 

#### (c) What are some real-world software examples that would need the above routines? Why? Would they benefit greatly from using your distributed code?
        
        This library would be usefull for anyone conducting any largre matrix operations and linear algerbra.
        Some specific exsamples would be video games and literally any feild that uses linear algerbra. 
        Although our code will really show it's usefullness in high rementions it will work in every dimenssion. 
        Some econmics will use linear algerbra one exsample being the Leontief input-output model.
        
#### (d) How could the code be improved in terms of usability, efficiency, and robustness?
        
        One major edge case we took into consideration was the nunmber of nodes won't evenly split our data.
        Or we have to many nodes. But the way we constructed our code if you give it too many nodes it will
        simply ignore the extra ones. This makes it easier for an end user to not have to read every line of
        our code to see how many nodes to give. In the moment we don't see any more improvements but I am 
        sure when we look bakc on this program with more experience we will find plenty of improvements.
        
        
