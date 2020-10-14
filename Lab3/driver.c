#include<stdio.h> // I/O
#include<stdlib.h> // Standard library functions
#include<mpi.h> // MPI Library
#include<time.h> // Time functions
#include"matrix.h" // Matrix Library


int main(int argc, char** argv){
    srand(time(NULL));

    MPI_Init(&argc, &argv);

    MPI_Comm world = MPI_COMM_WORLD;

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize);
    MPI_Comm_rank(world, &myRank);

    int testCases[9] = {10, 80, 100, 300, 500, 800, 1000, 5000, 10000};
    int i, w;
    for(w=0; w<9; w++){
    
    const int DIMENSION = testCases[7]; // Change the test case matrix dimensions
    

    //These will be used through out all testing
    Matrix A;
    Matrix B;
    
    Matrix Result;

    //Testing addition
    /*
    int rowLength = DIMENSION;
    int colLength = DIMENSION;
    initMatrix(&A, rowLength, colLength);
    initMatrix(&B, rowLength, colLength);
    initMatrix(&Result, rowLength, colLength);
    free(Result.data);

    int i;
    for (i = 0; i < 10; i++) {
        Result.data = addMatrices(&A, &B, &world, worldSize, myRank);
        if(myRank == 0){
            puts("Done");   
            free(Result.data);
        }
    }
    if (myRank == 0) {
        free(A.data);
        free(B.data);
    }
   */
    

    
    //Testing subtraction
    /*
    int rowLength = DIMENSION;
    int colLength = DIMENSION;
    initMatrix(&A, rowLength, colLength);
    initMatrix(&B, rowLength, colLength);
    initMatrix(&Result, rowLength, colLength);
    free(Result.data);

    int i;
    for (i = 0; i < 10; i++) {
        Result.data = subtractMatrices(&A, &B, &world, worldSize, myRank);
        if(myRank == 0){
            puts("Done");    
            free(Result.data);
        }
    }
    if (myRank == 0) {
        free(A.data);
        free(B.data);
    }
    */ 
    



    //Testing Inner Product
    /*
    int ArowLength = 1;
    int AcolLength = DIMENSION;
    int BrowLength = DIMENSION;
    int BcolLength = 1; 
    initMatrix(&A, ArowLength, AcolLength);
    initMatrix(&B, BrowLength, BcolLength);
    initMatrix(&Result, ArowLength, BcolLength);
    free(Result.data);

    int i;
    double resultNum = 0;
    for (i = 0; i < 10; i++) {
        resultNum = innerProduct(&A, &B, &world, worldSize, myRank);
        if(myRank == 0){
            puts("Done");    
        }
    }
    if (myRank == 0) {
        free(A.data);
        free(B.data);
    }
    */ 



    //Testing Multiply
    /*
    int ArowLength = DIMENSION;
    int AcolLength = DIMENSION;
    int BrowLength = DIMENSION;
    int BcolLength = DIMENSION;    
    initMatrix(&A, ArowLength, AcolLength);
    initMatrix(&B, BrowLength, BcolLength);
    initMatrix(&Result, ArowLength, BcolLength);
    free(Result.data);

    int i;
    for (i = 0; i < 10; i++) {
        Result.data = multMatrices(&A, &B, &world, worldSize, myRank);
        if(myRank == 0){
            puts("Done");    
        }
        free(Result.data);
    }
     
    if (myRank == 0) {
        free(A.data);
        free(B.data);
    }
*/
    }
    MPI_Finalize();
    return 0;
}
