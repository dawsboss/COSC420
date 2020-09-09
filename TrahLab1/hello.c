#include<stdio.h> //printf
#include <mpi.h>

// module load mpi/openmpi3_x86_64
// mpicc filename.c
// mpiexec -n # ./a.out

int main(int argc, char**argv){
  MPI_Init(&argc,&argv);

  MPI_Comm world = MPI_COMM_WORLD;

  int worldSize, myRank;

  MPI_Comm_size(world, &worldSize);
  MPI_Comm_rank(world, &myRank);

  if (myRank == 0 ) {
    printf("Hello from master node!\n");
    printf("There are %d total nodes!\n", worldSize);
  }  else {
    printf("Hello from worker node %d!\n", myRank);
  }

  printf("Hello, from node %d!\n",myRank);
  //printf("Hello world!\n");
  
  MPI_Finalize();
  return 0;
}
