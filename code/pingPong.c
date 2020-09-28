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

    int token =0;
    int counter =0;

    if(worldSize != 2){
        fprintf(stderr, "ONLY RUN ME WITH 2 PROCESSORS\n");
        MPI_Finalize();
        return 0;
    }

    while(token < 10){
        if(myRank%2 == token%2){
          token++;  
          printf("proc %d sent %d to %d\n", myRank, token, (myRank+1)%2);
          MPI_Send(&token, 1, MPI_INT, (myRank+1)%2, 0,  world);
        }else{
            MPI_Recv(&token, 1, MPI_INT, (myRank+1)%2 , 0, world, MPI_STATUS_IGNORE);
            printf("proc %d received %d from %d\n", myRank, token, (myRank+1)%2);
        }
    }

    





    MPI_Finalize(); // Finalizing MPI

    return 0;
}
