#include<stdio.h> //printf
#include<math.h> //sqrt
#include <mpi.h>

int main(int argc, char**argv){
  if(argc > 1){
    puts("Only one argument needed (N)");
    return 69;
  }
  MPI_Init(&argc,&argv);

  MPI_Comm world = MPI_COMM_WORLD;

  int worldSize, myRank;

  MPI_Comm_size(world, &worldSize);
  MPI_Comm_rank(world, &myRank);

  sqrtN = sqrt(N)

  if(worldSize >= sqrtN){
    puts("You gave more nodes than needed, maxc number of nored needed is sqrt(N) or in your case: " + sqrtN);
  }

  numofStuffToDo = sqrtN/worldSize

  startingNumber = (myRank*numofStuffToDo)+1

  bool isPrime = true;
  for(int i = startingNumber; i <= startingNumber+numofStuffToDo; i++){
    if(N%i == 0){
      isPrime = false;
      break;
    }
  }
  return isPrime;

  MPI_Finalize();
  return 0;
}
