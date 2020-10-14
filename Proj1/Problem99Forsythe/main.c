#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<mpi.h>
#include<string.h>
#define WIDTH 2
#define HEIGHT 1000

long int POW(int base, int exp)
{
	int i; 
	long value=1;
	for(i=0; i < exp; i++)
	{
		value = value * base;	
	}
	//printf("Value: %ld\n",value);
	return value;	
}

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	MPI_Comm world = MPI_COMM_WORLD;
	int worldSize, myRank;
	MPI_Comm_size(world, &worldSize);
	MPI_Comm_rank(world, &myRank);
	
	if(argc < 1)
	{
		printf("Need an argument\n");
		MPI_Finalize();
		return 0;
	}
	//MPI_File fh;
	//int matrix[HEIGHT][WIDTH];
	int offset = worldSize*myRank*sizeof(int);
	//MPI_File_open(world, argv[1], MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	printf("Rank: %d, Offset: %d\n", myRank, offset);
	//MPI_File_read_at(fh,offset,nums,worldSize,MPI_INT,MPI_STATUS_IGNORE);
	printf("reading the file\n");
	FILE *fh;
	fh = fopen(argv[1], "r");
	if(fh == NULL)
	{
		printf("Error opening file\n");
		return 0;
	}
	int i;
	int localLen = HEIGHT/worldSize;
	int *baseArr = malloc(HEIGHT*sizeof(int));
	int *expArr = malloc(HEIGHT*sizeof(int));
	int *localBArr = malloc(localLen*sizeof(int)); //Holds all base valuse 
	int *localEArr = malloc(localLen*sizeof(int)); //Holds all exp values
	int *value = malloc(localLen*sizeof(long)); //Hodls result of above values
	for(i=0; i < HEIGHT;i++)
	{
		if(fscanf(fh, "%d,%d", &baseArr[i],&expArr[i]) < 0)
		{
			printf("Error reading file\n");
			return 0;
		}
	}
	for(i=0; i < HEIGHT; i++)
	{
		printf("Number %d is: %d , %d\n", i, baseArr[i], expArr[i]);
	}

	//MPI_Scatterv(baseArr, &localLen, &offset, MPI_INT, localBArr, localLen, MPI_INT, 0, world);
	//MPI_Scatterv(expArr, &localLen, &offset, MPI_INT, localEArr, localLen, MPI_INT, 0, world);
	MPI_Scatter(baseArr, localLen, MPI_INT, localBArr, localLen, MPI_INT, 0, world);
	MPI_Scatter(expArr, localLen, MPI_INT, localEArr, localLen, MPI_INT, 0, world);
	
	long int max=0;
	for(i=0; i < localLen; i++)
	{
		// Find the answer value
		value[i] = POW(localBArr[i], localEArr[i]);
		//printf("Value at %d: %d\n",i,value[i]);
		if(value[i] > max)
		{
			//replaces the max value with the current value if it is larger
			max = value[i];
		}
	}
	int maxVals[worldSize];
	//MPI_Gatherv(&max, localLen, MPI_INT, maxVals, &localLen, &offset, MPI_INT, 0, world);
	MPI_Gather(&max, localLen, MPI_INT, maxVals, localLen, MPI_INT, 0, world);
	for(i=0; i < worldSize;i++)
	{
		if(maxVals[i] >  maxVals[i+1])
		{	
			//printf("%d is greater than %d\n", maxVals[i], maxVals[i+1]);
			max = maxVals[i]; 
		}
	}

	//if(myRank == 0)
	//{
	printf("Largest exponential value: %ld\n", max);
	//}

	free(expArr);
	free(baseArr);
	free(localBArr);
	free(localEArr);
	free(value);
	fclose(fh);
	MPI_Finalize();
	return 0; 
}
