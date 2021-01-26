### Ryan Rosiak and Grant Dawson
COSC 420-001
12/15/20

# Project-3: arXiv Search Engine Continuation

Continue working with your Project 2 groups to improve your code to be a runnable document search engine.

### Remember Project 2:
In project 2:
1. You will create a search engine to search scientific articles stored on arXiv
2. This will consist of two components:
    (a) An index of test search terms, to know which documents are appropriate results to be returned
    (b) A ranking algorithm to determine which papers are the most "important", relative to the given search term. This will use the PageRank and HITS algorithms
3. You will use two data files: arxiv-metadata.txt and arxiv-citations.txt
4. Your project will need two main "modules"
5. Actual usage will be done by a single program

## Project Description:
Continue working with your Project 2 groups to improve your code to be a runnable document search engine. You should make at least one major algorithmic improvement to the code base; this can be in the form of a more scalable matrix multiplication routine, more advanced data structure, applying a series of improvements/strategies to get the full program to run efficiently on the entire arXiv dataset. Even if your final product is slow, it should be capable of handling the full dataset, given enough time. For example, take maximum advantage of file storage mechanisms to work around memory restrictions; use more time-inefficient techniques that would instead prevent memory or numerical problems. Your presentation should focus on the specific underlying improvements that you made on top of the original version of your code. Include benchmarks and measurements to illustrate isolated improvements, for instance of memory usage, time is taken to build data files, etc. Be sure to document what next steps could be taken to increase the scalability of the program.

This project demonstrates the usage of the PageRank algorithm. This algorithm
uses advanced linear algebra techniques in order to obtain useful numbers to perform
a hypertext-based search. This project also demonstrates the usage of memory management
as there is a copious amount of data to move around, load, and unload. Lastly, file
management and preprocessing are pivotal features of this large project. 

## Folders to pay attention to:

BST, CitationFile, MetadataFile, PageRank - These are the folders containing active modules

## How to Run:

See README's in each folder to run each module separately. Currently, a final build of
this project bringing all of the data together is not complete.

1. Run the the writedata executable in the MetadataFile module (Needs arxiv-metatdata.txt)

2. Run the the writematrix executable in the CitationFile module (Needs arxiv-citations.txt)

3. Run the main executable in the PageRank module (Tests are shown for proof of correctness, but takes a very long time). To continue testing, run the run executable to get a sample file of pageranks

4. Run the main executable in the MainProgram module (Currently not finished, see issues section below to learn what is wrong)

## What we had before starting:
#### PageRank:
* Semi-working code - The page Rank didn't work in parallel... 
* Inefficient concepts - The code I had was very inefficient (the end product could be better as well)
* Scatterbrain - I can explain this in one analogy, I created a boat as fast as possible and put it on the water, and tried repairing the holes while on the water. 
* Too close to the topic - I needed space form the topic to get a new view
* Not integrated to take input from files - This is what we ended up needing since we could load the matrix in memory.
##### Files:
* Complete Meta-Data
    * Structs of metadata for papers
*  Complete Index
    * Structs of paper ID and given unique index given by us
*  Garbage Adjacency Matrix
*  Binary Tree class

## What we are ending with:
#### PageRank:
* Fully paralized code
* Takes input from file
* Equipt to take sparse matrices

![Eigen Vector](https://github.com/spa542/COSC420/blob/master/Project3P2Continuation/Time%20vs.%20Number%20of%20Nodes.png)

#### Files: 
* Sparse Adjacency Matrix
* PageRank File
    * Structs of IDs and papers PageRank
* User Interface

## Issues :
* Memory space
    * Solution: Only load in what we need as we need it. Put the load on the hard drive - We filled our RAM extremely fast because we attempted to simply load a 1.7mil x 1.7mil matrix into the memory of doubles all at once. So we shifted to the idea that we would work row by row and save the idle data into the hard drive.
* Then hard drive space
    * Solution: Double -> short -> char -> Bit masking -> Sparse Matrices - This is the evolution of what we did as a solution for saving the adjacency matrix. Where we slowly lowered the amount of hard drive space we used, 30T -> 8T -> 3T -> 43GB -> 35GB. And yes that is from 30 Terabytes to a 35 GB file, and instead of us having to learn bit masking to get to 40Gb we made our protocol which makes us not have to save each 0 and 1 individually. The 1's are implied with gaps of numbers and we just count the number of zeros between 1's
* Change code
    * We have this new solution but it is now using a new protocol of Sparse Matrices - This wasn't the biggest deal but it still meant time we had to re-code working code so that we can use this new protocol.
* Unclean data
    * Missing papers from citation file - the given metadata file had all the papers in it but the citation file was missing some of the papers cause for us to make our algorithms that read the files have to be very timmid and over engineered. 
* Main Module incomplete
    * We have been working on a solution on creating a backwards index but have not gotten the chance for fully implement it for production. The BST's work and are ready but more needs to be carved out so the main module works fluidly.

## Where are we left? What next?
* We plan to finish this code. We plan to have a fully functioning program. 
* Make the code travel size
    * This involves us making the code both more portable and condensed. This allows for an easier user interface, changes in data it would read in so one day it would be able to work on bigger and smaller data sets with raw data files with the same protocols. 
* Refining UI
    * No user input is allowed (yet)

