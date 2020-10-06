#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv); // Initializing MPI

    MPI_Comm world = MPI_COMM_WORLD; // Communicator

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize); // Gives amount of nodes running in parallel
    MPI_Comm_rank(world, &myRank); // Gives the rank (number) node
    // MPI_Get_processor_name(processorName, &processSize); 
    srand(time(0));                           // Gives the processor name of current node

    





    MPI_Finalize(); // Finalizing MPI

    return 0;
}
