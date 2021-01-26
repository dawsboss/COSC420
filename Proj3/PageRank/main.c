#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include"PageRank.h"

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME

int main(int argc, char** argv) {

    MPI_Init(&argc, &argv); // Initializing MPI

    MPI_Comm world = MPI_COMM_WORLD; // Communicator

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize); // Gives amount of nodes running in parallel
    MPI_Comm_rank(world, &myRank); // Gives the rank (number) node
    srand(time(0));      
    int i;



    //Test 1
    if(myRank == 0){
      int arr[] = {
      2,0,0,0,
      4,0,-1,-1,
      4,0,-1,-1,
      4,0,-1,-1,
      5,-1,-1,-1
      };
      int* tmp = (int*)malloc(5*4*sizeof(int));
      for(i=0; i<5*4; i++){
          tmp[i] = arr[i];
      }

      FILE* fh = fopen("matrixtestfile", "w");
      fwrite(tmp, sizeof(int),20, fh);
      fclose(fh);
      free(tmp);
    }
    
   double* rtn = pageRank(5, 4, &world, worldSize, myRank);
    
    
    if(myRank == 0){
      for(i=0; i<5; i++){
        printf("%f ", rtn[i]);    
      }
      puts("");
    }

    free(rtn);
   

    //Test 2 
    if(myRank == 0){
      int arr[] = {
      0,0,0,0,6,
      6,0,0,0,0,
      10,-1,-1,-1,-1,
      0,0,8,-1,-1,
      5,4,-1,-1,-1,
      0,6,0,0,0,
      0,9,-1,-1,-1,
      1,2,5,-1,-1,
      10,-1,-1,-1,-1,
      10,-1,-1,-1,-1
    };
      int* tmp = (int*)malloc(10*5*sizeof(int));
      for(i=0; i<10*5; i++){
          tmp[i] = arr[i];
      }

      FILE* fh = fopen("matrixtestfile", "w");
      fwrite(tmp, sizeof(int),50, fh);
      fclose(fh);
      free(tmp);
    }
    
   rtn = pageRank(10, 5, &world, worldSize, myRank);
    
    
    if(myRank == 0){
      for(i=0; i<10; i++){
        printf("%f ", rtn[i]);    
      }
      puts("");
    }

    free(rtn);

    MPI_Finalize(); // Finalizing MPI

    return 0;
}
