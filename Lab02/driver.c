#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include"matrix.h"


int main(int argc, char** argv){
    srand(time(NULL));

    MPI_Init(&argc, &argv);

    MPI_Comm world = MPI_COMM_WORLD;

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize);
    MPI_Comm_rank(world, &myRank);

    //These will be used through out all testing
    Matrix A;
    Matrix B;
    
    Matrix Result;

    //Testing addition
    /*int rowLength = 2560;
    int colLength = 8600;
    initMatrix(&A, rowLength, colLength);
    initMatrix(&B, rowLength, colLength);
    initMatrix(&Result, rowLength, colLength);
    free(Result.data);

    Result.data = addMatrices(&A, &B, &world, worldSize, myRank);
    if(myRank == 0){
        puts("Done");   
    }

    free(A.data);
    free(B.data);
    free(Result.data);
    
    
    
    
    
    */
    //Testing subtraction
    /*
    int rowLength = 2560;
    int colLength = 8600;
    initMatrix(&A, rowLength, colLength);
    initMatrix(&B, rowLength, colLength);
    initMatrix(&Result, rowLength, colLength);
    free(Result.data)

    Result.data = subtractMatrices(&A, &B, &world, worldSize, myRank);
    if(myRank == 0){
        puts("Done");    
    }

    
    free(A.data);
    free(B.data);
    free(Result.data);
    
    



    */
    //Testing Inner Product
    
    int ArowLength = 3820;
    int AcolLength = 1;
    int BrowLength = 3820;
    int BcolLength = 1;    
    initMatrix(&A, ArowLength, AcolLength);
    initMatrix(&B, BrowLength, BcolLength);
    initMatrix(&Result, ArowLength, BcolLength);
    free(Result.data);

   int Data = innerProduct(&A, &B, &world, worldSize, myRank);
    if(myRank == 0){
        puts("Done");    
    }
 
    free(A.data);
    free(B.data);
    //free(Result.data);
    
    



    
    //Testing Multiply
    /*
    int ArowLength = 2560;
    int AcolLength = 8600;
    int BrowLength = 8600;
    int BcolLength = 3820;    
    initMatrix(&A, ArowLength, AcolLength);
    initMatrix(&B, BrowLength, BcolLength);
    initMatrix(&Result, ArowLength, BcolLength);
    free(Result.data)

    Result.data = multMatrix(&A, &B, &world, worldSize, myRank);
    if(myRank == 0){
        puts("Done");    
    }
     
    free(A.data);
    free(B.data);
    free(Result.data);
    
    */

    MPI_Finalize();
    return 0;
}
