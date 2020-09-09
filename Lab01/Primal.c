#include<stdio.h> // Standard output
#include<stdlib.h> // Standard library
#include<math.h> // sqrt
#include<mpi.h> // MPI functions
#include<time.h> // Time functions
#include<stdbool.h> // bool
#include<unistd.h> // sleep

int main(int argc, char** argv) {

    if (argc != 2) {
        puts("Invalid Arguments, ./Primal ( num )");
        return 1;
    }

    MPI_Init(&argc, &argv);

    MPI_Comm world = MPI_COMM_WORLD;

    unsigned long int endValue = atoi(argv[1]); // # of values to be checked
    int worldSize, myRank, maxJobs, stuffToDo, startingNum;

    MPI_Comm_size(world, &worldSize); // Amount of nodes being used
    MPI_Comm_rank(world, &myRank); // Rank of each node

    maxJobs = sqrt(endValue); // Amount of numbers that need to be checked

    if (worldSize >= maxJobs) { // Only need as many nodes as there are jobs for efficiency
        printf("Amount of nodes exceeds the amount for the job. # of nodes needed is sqrt(%d)", maxJobs);
        MPI_Abort(world, 1);
        return 1;
    }

    stuffToDo = maxJobs / worldSize; // Numbers that each node has to check

    if (myRank == 0) {
        startingNum = 2; // 0 and 1 are unnecessary
    } else {
        startingNum = myRank * stuffToDo + 1; // Where each node will start
    }

    // Time the Nodes as they do their work
    time_t before = time(NULL);

    int i;
    bool foundAnything = false;
    printf("Node %d is performing on ranges %d to %d\n", myRank, startingNum, startingNum + stuffToDo);
    for (i = startingNum; i <= startingNum + stuffToDo; i++) {
        if (endValue % i == 0) { // Checks for prime
            printf("%ld is composite, divisible by %d\n", endValue, i);
            foundAnything = true; 
            break;
        } 
    }

    if (!foundAnything) {
        printf("Node %d believes that %ld is prime...\n", myRank, endValue);
    }

    time_t after = time(NULL);

    printf("Number of seconds for node %d to compute if %ld was prime is %f\n",
            myRank, endValue, difftime(after, before));

    MPI_Finalize();

    return 0;
}
