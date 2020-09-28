### Ryan Rosiak and Grant Dawson
COSC 420-001
9/7/20

# Lab-2: Matrices
* Develop familiarity with C programming language
* Develop familiarity with parallel computing tool OpenMPI
* Explore empirical tests for program efficiency in terms of parallel computation and resources
 

## Lab Description:
This lab demonstrates parallel matrix functions. We implemented every basic
matrix operation such as addition, subtraction, multiplication, and inner 
product with the help of parallel computing. These functions go beyond
basic functionality by using MPICH to divide up our matrices during 
computation in order to make our functions faster.

## Questions:




#### (a) What is the theoretical time complexity of your algorithms (best and worst case), in terms of the input size?
#### (b) According to the data, does adding more nodes perfectly divide the time taken by the program?
#### (c) What are some real-world software examples that would need the above routines? Why? Would they benefit greatly from using your distributed code?
        This library would be usefull for anyone conducting any largre matrix operations and linear algerbra. Some specific exsamples would be video games and literally
        any feild that uses linear algerbra. ALthough our code will really show it's usefullness in high rementions it will work in every dimenssion. Some econmics will use 
        linear algerbra one exsample being the Leontief input-output model.
#### (d) How could the code be improved in terms of usability, efficiency, and robustness?
        One major edge case we took into consideration was the nunmber of nodes won;t evenly split our data. Or we have to many nodes. But the way we constructed our code 
        if you give it too many nodes it will simply ignore the extra ones. This makes it easier for an end user to not have to read every line of our code to see how many 
        nodes to give. In the moment we don't see any more improvements but I am sure when we look bakc on this program with more experience we will find plenty of improvements.
