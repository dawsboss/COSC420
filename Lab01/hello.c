#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv); // Initializing MPI

    MPI_Comm world = MPI_COMM_WORLD; // Communicator

    int worldSize, myRank;
    char processorName[pNameSize];
    int processSize = pNameSize;

    MPI_Comm_size(world, &worldSize); // Gives amount of nodes running in parallel
    MPI_Comm_rank(world, &myRank); // Gives the rank (number) node
    MPI_Get_processor_name(processorName, &processSize); 
                                // Gives the processor name of current node

    if (myRank == 0) {
        puts("Hello from the master node!");
        printf("There are %d total nodes!\n", worldSize);
    } else {
        printf("Hello from worker node %d!\n", myRank);
    }

    printf("Hello, from node %d! My processor name is %s\n", myRank, processorName);

    MPI_Finalize(); // Finalizing MPI

    return 0;
}
