#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mpi.h>
#include"matrix.h"

int main() {
    
    // For problem 2
    /*
    Matrix a = default_matrix;
    a.rows = a.cols = 8;
    a.data = (double*)malloc(a.rows*a.cols*sizeof(double));
    memset(a.data, 0, a.rows*a.cols*sizeof(double));
    Matrix *yeet = &a;
    a.data[INDEX(yeet, 0, 1)] =
    a.data[INDEX(yeet, 0, 3)] = 
    a.data[INDEX(yeet, 0, 6)] = 
    a.data[INDEX(yeet, 0, 7)] = 
    a.data[INDEX(yeet, 1, 0)] = 
    a.data[INDEX(yeet, 1, 2)] =
    a.data[INDEX(yeet, 1, 4)] = 
    a.data[INDEX(yeet, 1, 5)] = 
    a.data[INDEX(yeet, 2, 0)] = 
    a.data[INDEX(yeet, 2, 1)] =
    a.data[INDEX(yeet, 2, 2)] =
    a.data[INDEX(yeet, 2, 7)] = 
    a.data[INDEX(yeet, 3, 0)] = 
    a.data[INDEX(yeet, 3, 2)] = 
    a.data[INDEX(yeet, 3, 3)] =
    a.data[INDEX(yeet, 3, 4)] = 
    a.data[INDEX(yeet, 3, 7)] = 
    a.data[INDEX(yeet, 4, 1)] = 
    a.data[INDEX(yeet, 4, 2)] = 
    a.data[INDEX(yeet, 4, 3)] = 
    a.data[INDEX(yeet, 4, 6)] = 
    a.data[INDEX(yeet, 5, 5)] = 
    a.data[INDEX(yeet, 5, 6)] = 
    a.data[INDEX(yeet, 6, 0)] = 
    a.data[INDEX(yeet, 6, 1)] = 
    a.data[INDEX(yeet, 6, 3)] = 
    a.data[INDEX(yeet, 6, 4)] = 
    a.data[INDEX(yeet, 6, 6)] = 
    a.data[INDEX(yeet, 6, 7)] = 
    a.data[INDEX(yeet, 7, 0)] = 
    a.data[INDEX(yeet, 7, 2)] = 
    a.data[INDEX(yeet, 7, 3)] = 
    a.data[INDEX(yeet, 7, 4)] = 
    a.data[INDEX(yeet, 7, 6)] = 1;

    FILE* fh = fopen("graphmatrix", "w");
    fwrite(a.data, sizeof(double), a.rows*a.cols, fh);
    fclose(fh);


    free(a.data);
    a.data = (double*)malloc(a.rows*a.cols*sizeof(double));
    memset(a.data, 0, a.rows*a.cols*sizeof(double));
    fh = fopen("graphmatrix", "r");
    fread(a.data, sizeof(double), a.rows*a.cols, fh);
    printMatrix(&a);
    fclose(fh);
    */
    MPI_Init(NULL, NULL);
    
    MPI_Comm world = MPI_COMM_WORLD;
    int myRank, worldSize; 
    MPI_Comm_rank(world, &myRank);
    MPI_Comm_size(world, &worldSize);

    Matrix yeet = default_matrix;
    yeet.rows = 8;
    yeet.cols = 1;
    yeet.data = NULL;
    yeet.data = EigenVector("graphmatrix", 8, &world, worldSize, myRank);

    if (myRank == 0) {
        printMatrix(&yeet);
    }

    // I was working with the eigenvector. The formula is Av = eigval*v
    // If I multiply A*v then I can use that answer, and find the constant c
    // that makes c*v = A*v. This c will be our eigenvalue
    /*
    Matrix mat = default_matrix;
    mat.rows = mat.cols = 8;
    mat.data = (double*)malloc(mat.rows*mat.cols*sizeof(double));
    FILE* fh = NULL;
    fh = fopen("graphmatrix", "r");
    fread(mat.data, sizeof(double), mat.rows*mat.cols, fh);
    Matrix
    */


    MPI_Finalize();

    return 0;
}
