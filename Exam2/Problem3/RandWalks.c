#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include"matrix.h"

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

    int i,j;
    
    double arr[8*8]={0,1,0,1,0,0,1,1,
                    1,0,1,0,1,1,0,0,
                    1,1,1,0,0,0,0,1,
                    1,0,1,1,1,0,0,1,
                    0,1,1,1,0,0,1,0,
                    0,0,0,0,0,1,1,0,
                    1,1,0,1,1,0,1,1,
                    1,0,1,1,1,0,1,0};


    Matrix At = default_matrix;
    Matrix* A = &At;
    A->rows = A->cols = 8;

    double* Adata = (double*)malloc(A->rows*A->cols*sizeof(double));

    for(i=0; i<A->rows*A->cols; i++){
        Adata[i] = arr[i];
    }

    A->data = Adata;

    int local_v[8] = {0,0,0,0,0,0,0,0};//Holds how many times we stopped on count[i]s node 0-9 = 1-10
    
    //step d
    for(i=0; i<1000; i++){//WOuld add the worldSize dirty business so each processor does a chunk of the 1000 itterations asked of us
        //step a
        int nodeS = rand()%8;//TODO check
        
        //step b
        int k = 1+ rand()%8;//TODO check

        //step c
        int current = nodeS;
        int itter = 0;
        int done = 0;//0no : 1yes
        int next=0;

        while(itter<k){
            while(done!=1){//Find a number we can jump to randomly
                next = rand()%9;
                for(j=0; j<A->rows; j++){
                    if(ACCESS(A,current,next) == 1){
                        done = 1;
                        break;
                    }           
                }
            }

            current = next;
            itter++;
        }
        local_v[current]++;
   }

    for(i=0; i<8;i++){
        printf("Node: %d | hit: %d\n", i, local_v[i]);
    }
    

    //MPI_Gatherv();I would have gathered everyones matrixies at the end and then combinesd them so I had a single vector that said how many time I landed on * node in the end.
    //I would then go on to add at the top and make it so each processor that was givin in world size would do the same number of the 1000 itterations or off by one because not divisiable. 


    MPI_Finalize(); // Finalizing MPI

    return 0;
}
