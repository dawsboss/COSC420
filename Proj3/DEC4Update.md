### Ryan Rosiak and Grant Dawson
COSC 420-001
12/04/20

# Project-3: arXiv Search Engine Update
Files we updated:
1.  PageRank
2.  Citation File

## PageRank
In PageRank since our last update, which was the second project (which is in the GitHub), 
I have completely revamped the code. We are starting from scratch to ensure that the 
code's ideas are sound and that I know how I will parallelize it before I start programming.
So the code in PageRank.c does not show much actual progress, but I have been making sure
the theory is down before programming. So as of submission for this check-up, I have a
theory all ready, and all that is limiting is time. This upcoming weekend(this was 
written on a Friday), I plan to add a lot to all the code for the PageRank algorithm. 
It is good to note that t isn't a simple copy and paste for most of the code because we
are now using the idea of sparse matrices to save storage space, but this leads to changes.
Also, the code from before did not work and was not parallelized. But the theory right now 
is we will have the code parallelized at a high level, so all the processors are not doing 
the same operations as previous programs, but currently, they are all working on one enormous
task with smaller operations. So each one will be conducting matrix multiplications and 
additions on their own. As a matter of fact, each processor will take a separate row than 
any other processor from the adjacency matrix. 

## MetaFile Scripting
Currently, the algorithm for reading in the citations and creating the matrix has been tweaked
to be used with sparse matrices. The speed up is significant because we are only storing 
padded rows of integers where each element represents the number of 0s between each 1 in the 
matrix. This file is currently being ran in full on the cluster and should be confirmed as 
finished within a day. That is the main change because there were some extra functions 
regarding sorting the indices and special calculations needed to make the algorithm 
work effectively.

## Going Forward
Overall, once we have both the sparse matrix file and the pagerank/hits working, we are going
to create a script that will bring all of the calculated data paired with each paper into a struct
and write those into a file. From there, we will use that metadata along with the already calculated
metadata from the other paper script to bring it all together so that we can create one large struct 
for each paper that has all of the information neat and tidy in one spot to be used in the actual
program algorithm.
