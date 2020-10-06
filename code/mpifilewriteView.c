#include<stdio.h> // printf
#include<mpi.h>
#include<time.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

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

  if(argc < 2){
    fprintf(stderr, "Need 2 arguments!\n");
    MPI_Finalize();
    return 0;
  }

  srand(time(0)+myRank);

  int N = atoi(argv[1]);
  int nums[N], i;
  char buf[N*3 + 1];
  memset(buf, '\0', N*3 + 1);
  char tmp[4];

  MPI_File fh;
  int offset = N*myRank*sizeof(int);
 
  for(i=0; i<N; i++){
    nums[i] = rand()%100;
    sprintf(tmp, "%d ", nums[i]);
    strcat(buf, tmp);
  }

  printf("Rank %d numbers:\n%s\n", myRank, buf);

  MPI_File_open(world, "datafile",
      MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);

  printf("Rank: %d, Offset: %d\n", myRank, offset);

  // hexdump -v -e '5/4 "%3d"' -e '"\n"'  datafile
  //'10/4' 
  //10 - every 10 numbers / number fo columns
  // 4 - number of bytes per item, an int is 4 bytes
  MPI_File_set_view(fh, offset, MPI_INT, MPI_INT, "native", MPI_INFO_NULL);
  MPI_File_write(fh, nums, N, MPI_INT, MPI_STATUS_IGNORE);

  MPI_File_close(&fh);

  MPI_Finalize();
  return 0;
}
