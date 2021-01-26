# Project 2 - Page Rank Algorithm - Grant Dawson
  * PageRank
  * HITS
  * File PageRank
  * File HITS

## Usage: (to test)
First of all, the final answers are similarly printed out. 
    
    make
    ./main (OR) mpirun -n # ./main (OR) sbatch P160.sh
    

## Analysis of the code:
There are 4 separate functions I will analyze all 1000 lines of code in PageRank.c

#### PageRank (Regular):
 This code is from the idea we derived form class where we have the equation 
 
    alpha( M*P ) + Ones(1-alpha)
 
 Where alpha is the chance of teleporting or the user changing pages entirely out of the line
 M being the stochastic Matrix by row from the Adjacency matrix
 P being the end answer being affected and starts at all 1's 
 Ones is a matrix of all ones
 
 This code is when its memory is not a concern for the M matrix and adjacency matrix. 
 It is very wasteful and has the expectation that it can hold all the data in memory.
 Although not the worst assumption, it assumed too much for the end game of the project.
 There are slight issues with this code that I found but did not bother to fix because in the 
 long-run as you will see farther down the readME, this will not be used for the final project. 
 
 This code will Loop until the 'P' matrix converges that or it loops for 2500 times. So the big O of this code comes from the matrix operation we made in the Matrix. C file. The code calls Mathrix to multiply, Matrix adds, Matrix subtract, and normalize the whole Adjacency matrix.
 So the bigO is related to those functions added up times a constant.
 
 The codes have only logical issues. It gets the wrong answer occasionally when it comes down to long counts, where it will never converge or should go out of the 2500 bounds to find the answer. It finishes short. This is an easy fix, but I did not have time to fix this since it is not essential for the end project.
 
#### File PageRank:
  This code follows the same equations as above with the same parameters, but now we do it in
  a new way so that multiple CPUs can do sperate work instead of them all working on the same 
  iteration. So instead of shortening the Big O's for the matrix operations, they work on 
  sperate rows, so they use the serial version of all matrix operations that would otherwise be needed.
  
  This code works terrifically! Minus if you want more than one node working on it currently. At the moment of submission, the code will get the right answer no matter the edge case (I used python to check my answers). But the new method of paralyzation isn't the primary purpose of this implementation of the page rank algorithm. The primary aim was MEMORY. Every project with MPI memory always makes it 10 times harder. This time, the project's end goal was to run the PageRank algorithm on 1.7 million by 1.7 million Matrix. If each number in the Matrix was a char, so 1 byte, this would still take up more than 2000 gigs of memory... That is an issue. So
  This code's solution is taking some of that pain and giving it to the hard drives, not in the form of the swap but in the way of file reading and writing. Instead of loading that whole Matrix into the ram to use it, we put it in a file and read in row by row for what we need. Each processor will read in a separate row, and in the end, each row will read in an iterated on. Once we go through the File once we check to see if it converged and determine if we need to start the process over or if we can give that current P vector as the answer.
  
  This code will run very similar to the previous iteration with small amounts of overheard from the File
  reading, but it will be gained since each processor is doing its own thing, not needing to wait on others.
  
  This code so far does not play well with multiple nodes running it and communicating, but we are working 
  on that but as of now, one node will give correct answers.

#### HITS (Regular):
  This code performs analysis on the Adjacency matrix just like page rank but at a different level. It analyses the hub and authority scores. So the scores are given for point to the most papers, and the score presented for getting pointed to the most respected. This is useful for ranking in some cases, but we don't actually use it for our orders. It is to show other methods. Then and we can code :)
  
  This code follows the equations.
  
    (L@L.T)@x = x
    (L.T@L)@y = y
  
  where L is the Adjacency matrix
  L.T is L transpose
  x is the hub score
  y is the authority score
  
  This is similar to solving Eighen vectors more than PageRank, but they both follow the same idea about looping until convergence (or a number). This time has fewer matrix operations making it "faster."
  But it is less cool of a ranking system than PageRank.
  
  This code uses Matrix multiplication and subtraction. Both only once, but the Big Os are similar.
  
  This code works for all the cases I tested on it.

#### File HITS:
  This code is similar to both HITS and PageRank files. It is similar to HITS. After all, it uses the same equations but similar to File PageRank because it uses the same iteration ideas. Except for this time, 
  it did not work as well. I thought there would be an easier translation of form on to another, and I 
  rank lout of time for this. This code does not work at all as of now, but this is because I put it off to work on the function the project needs to work/PageRank File.








#### Main:
The main is just a testing file of the PageRanks and HOTS versions to show how they compare to one 
another.


## The problems:
#### Problem 1: Bugs
This project's main issues were just general bugs with segfaulting and such. The main issues
stemmed from the version of the File of the code. The other ones were actually easy to work on. The file
version introduced several chunks for issues to arise. From nodes communicating their results and
giving them all back combined. That and the fact I then had to think of the problem split twice.
Once for the nodes/processes working on doing the math separate from one another. So prudent file management. The second split was the fact the rows would be split up, then I could only read in one row at a time to do this linear algebra I have been doing with an entire matrix both
in class, notes, and in my head. SO I had to get this new linear algebra concept to the problem making sense in my head. This new way of vectors multiplying giving a single number then combining them all. This is most of the issues within the last week but will be mentioned in every other problem.

#### Problem 2: Memory
This is Grant, about 2 weeks out from the due date looking in on the PageRank and the HITS algorithm he had made all excited. He boasted to his professor to be hit by a realization that night that the data set they were working on had a massive Adjacency matrix. So large, in fact, that it would not be able to load into memory. This meaning that the code that I took so long to make meant
nothing and was useless. So this was a massive drawback because yes, what I had was what I would use later. I still had to go back through all the code and practically remake it because it was new algebra behind it. So I sat down on a Saturday for 8 hours, crafting up a masterpiece.
In the end, it is not able to use more than one processor. This saddened me but made me glad it worked for one. Gave me hope that it was some sort of MPI issue I made or that I am close, and I 
just need more time. The unfortunate part is that I don't have more time until the project is due. 


 
### solution to problems:

The leading solution is time. I would have loved to have had my partner and I go over every section together and code together and have two brain work on one problem. But we had too much work to do that. We had too much to make to have both minds working on the same thing. That meant one reason wouldn't
be typing more lines of code. So with more time, we would have worked together and talked about the issues out. 

## Results:
The code works in some cases, and in the circumstances, it works, it does well. You will get your page rank. I 
can't tell you it will be fast and use your cluster, but it will give a PageRank. 

## Reflection:
This project is amazing. I love this idea as a project. It would also look amazing on a portfolio. But the issue is that there is so much to be done, and this would be manageable if I didn't have 15 credits worth of courses staring me in my face for attention to do work on. I will say that it is draining to work on, but I will also say that I don't regret it. I have learned a love for this, for C, for MPI,
for programming. This project has given me a new look, and it has brought me farther into the computer science world. By collaborating, researching, thinking, everything I need for a career. This
project has tested it.





