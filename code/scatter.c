#include<stdio.h>// printf
#include<stdlib.h>// srand
#include<mpi.h>
#include<time.h>// Time
#include<unistd.h>// sleep
#include<string.h>// strcar


// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME

int main(int argc, char** argv) {

    if( argc < 2 ){
      printf("Usage: ./scatter N\n");
      MPI_Finalize();
      return 1;
    }


    MPI_Init(&argc, &argv); // Initializing MPI

    MPI_Comm world = MPI_COMM_WORLD; // Communicator

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize); // Gives amount of nodes running in parallel
    MPI_Comm_rank(world, &myRank); // Gives the rank (number) node
    //MPI_Get_processor_name(processorName, &processSize);     // Gives the processor name of current node
    
    
    srand(time(NULL)+myRank);

    int N = atoi(argv[1]);
    
    if(N % worldSize != 0){
      printf("Make sure N is divisable by world size!\n");
      MPI_Finalize();
      return 2;
    }


    
    double* arr = NULL;
    
    int i=0;
    if(myRank == 0){
     arr = (double*) malloc(N*sizeof(double));

      for(i=0; i<N; i++){
        arr[i] = (double)rand() / RAND_MAX; //0 to 1
        printf("%0.2f ", arr[i]);
      } 
      puts("");
    }
    
    int local_len = N/worldSize;
    double* local_arr = (double*) malloc(local_len*sizeof(double));

    MPI_Scatter(
        arr, local_len, MPI_DOUBLE, //Sending info
        local_arr, local_len, MPI_DOUBLE, //Receiving info
        0, world); // book keeping and misc
    
    char* buf = (char*) malloc(5*local_len + 1);
    memset(buf, 0, 5*local_len + 1);
    /*
    for(i=0; i<5*local_len+1; i++){
      buf[i] = 0;
    }
  */

    char tmp[6];
    
    for(i=0; i<N; i++){
      sprintf(tmp, "%0.2f ", local_arr[i]);
      strcat(buf, tmp);
    }

    printf("I am node %d and my is %s", myRank, buf);


    free(local_arr);
    
    if(myRank == 0){
      free(arr);
    }
    free(buf);
    
    MPI_Finalize(); // Finalizing MPI
    return 0;
}
