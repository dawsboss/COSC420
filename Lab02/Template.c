#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME


//Precompiler macro
//A needs to be a struct not a POINTER!!
#define INDEX(A,i,j) A->cols*i + j
//This allows us to do A.data[INDEX(A,i,j)]
//companion macro
#define ACCESS(A,i,j) A->data[INDEX(A,i,j)]


typedef struct{
  int rows, cols;
  double* data;
} matrix;





void initMatrix(matrix *A){
  int i,j;
  A->data = malloc(A->rows*A->cols*sizeof(double));
  for(i=0;i<A->rows;i++){
    for(j=0;j<A->cols;j++){
      ACCESS(A,i,j) = (double) rand() / (double) RAND_MAX;
    }
  }
}


void printMatrix(matrix *A){
  int i,j;
  for(i=0;i<A->rows;i++){
    for(j=0;j<A->cols;j++){
      printf("%0.2f", ACCESS(A,i,j));
    }
    puts("");
  }
}




int main(int argc, char** argv) {

    MPI_Init(&argc, &argv); // Initializing MPI

    MPI_Comm world = MPI_COMM_WORLD; // Communicator

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize); // Gives amount of nodes running in parallel
    MPI_Comm_rank(world, &myRank); // Gives the rank (number) node
    //MPI_Get_processor_name(processorName, &processSize); 
                                // Gives the processor name of current node
    srand(time(0));


    matrix A;//TODO: allocate then init
    //to get A[i][j] we need data[cols*i + j]
    //we would need to say A.data[A.cols*i+j] - This is ugly use macros
    
    A.rows = 3;
    A.cols = 4;  
     
    initMatrix(&A);
    printMatrix(&A);

    MPI_Finalize(); // Finalizing MPI

    return 0;
}
