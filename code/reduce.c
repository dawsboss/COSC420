#include<stdio.h> // printf
#include<stdlib.h> // atoi
#include<mpi.h>
#include<time.h>
#include<string.h>


/* Run stuff:
 *  module avial
 *  module load *
 *  mpicc *filename*
 *  mpiexec -n *# of nodes* ./running* *args*
 */


// take a command-line arg, N, to denote
// a positive int
// ./a.out N
// has args: ./a.out and N
int main(int argc, char** argv){
  MPI_Init(&argc, &argv); // pass through cli args

  // This constant gets set by the MPI lib
  MPI_Comm world = MPI_COMM_WORLD;

  // worldSize will be the total number of
  // nodes in the communicator.
  // myRank will be the nodes id within that
  // communicator
  int worldSize, myRank;

  MPI_Comm_size(world, &worldSize);
  MPI_Comm_rank(world, &myRank);

  /** Begin real work here **/

  if(argc < 2){
    printf("Usage: ./a.out N\n");
    MPI_Finalize();
    return 1;
  }

  // the number of numbers to generate
  int N = atoi(argv[1]);

  // offset by rank so that each has a different seed
  srand(time(NULL) + myRank);

  // NB: allocation in C does not use "new"
  // Have to use "malloc" (or relatives) and "free"
  int* numbers = (int*) malloc(N*sizeof(int));
  int i;

/*Purely to allow printing the whole array
 * in a thread-safe way
 *
 *If we just printf one-by-one, the numbers will
 * mixed up betweem the nodes all competing for
 * my stdout.
 */

  
  //Only safe for N < 80 and 2 digit randoms
  char buf[256];
  char tmp[3];

  for(i=0; i<N; i++){
    numbers[i] = rand() % 10;
    sprintf(tmp, "%d ", numbers[i]);
    strcat(buf, tmp);
  }

  printf("I am node %d and my numbers are %s\n", myRank, buf);
    
  /* int MPI_Reduce(const void *sendbuf,
      void *recvbuf,
      int count,
      MPI_Datatype datatype, 
      MPI_Op op,
      int root, 
      MPI_Comm comm) */

  //All nodes set up the result pointer
  //but only the root writes to it
  int* result = NULL;
  
  if( myRank == 0){
    result = (int*) malloc(N*sizeof(int));
  }

  // EVERYBODY (including root) participates/calls this.
  // - The recvbuf only matters for the root
  // - computes the element-wise sum of each vector of N numbers
  // - stores result on the root's result array
  MPI_Reduce(numbers, result, N, MPI_INT, MPI_SUM, 0, world);

  if( myRank == 0 ){
    // ROOT SECTION
    for(i=0; i<N; i++){
      printf("%d ", result[i]);
    }
    puts("");
  }

  // de-allocate space
  free(numbers);
  free(result);

  /** End real work **/
  MPI_Finalize();
  return 0;
}
