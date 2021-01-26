#include<stdio.h> // I/O
#include<stdlib.h> // Standard library
#include<mpi.h> // MPI library
#include<time.h> // Time functions
#include"MatrixJawn/matrix.h" // File with matrix definitions and operations
#include"PageRank.h"

//This files is used for testing of the program


void writeTestFileSpecific(int dims, int tester, MPI_Comm* world, int worldSize, int myRank) {
    Matrix test = default_matrix;

    test.rows = test.cols = dims;
    int Varray[worldSize];
    int disp[worldSize];
    int i;
    for (i = 0; i < worldSize; i++) {
        Varray[i] = (dims*dims) / worldSize;
    }
    for (i = 0; i < (dims*dims) % worldSize; i++) {
        Varray[i] += 1;
    }
    test.data = (double*)malloc(Varray[myRank]*sizeof(double));
    for (i = 0; i < Varray[myRank]; i++) {
        test.data[i] = 0;
    }
    if(myRank == 0){
        if(tester == 0){
            test.data[1] = test.data[2] = test.data[3] = test.data[4] = test.data[6] = test.data[8] = test.data[11]  = 1;
        }else if(tester == 1){ 
            test.data[3] = test.data[1] = test.data[2] = 1;
        }else if(tester == 0){
            test.data[1] = test.data[2] = test.data[3] = test.data[7] = test.data[11] = 1;
        }
    }
    //test.data[11] = test.data[7] = test.data[3] = test.data[1] = test.data[2] = 1;

    //puts("test being dumb: ");
    //printMatrix(&test);

    int nextLength = 0;
    for (i = 0; i < worldSize; i++) {
        if (i == 0) {
            disp[i] = 0;
            nextLength = Varray[i];
            continue;
        }
        disp[i] = disp[i - 1] + nextLength;
        nextLength = Varray[i];
    }
    MPI_File fh;
    MPI_File_open(*world, "scripttest", MPI_MODE_CREATE | MPI_MODE_WRONLY,
            MPI_INFO_NULL, &fh);
    MPI_Offset off = disp[myRank]*sizeof(double);
    MPI_File_write_at(fh, off, test.data, Varray[myRank], MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);
    free(test.data);
}


// Script to write test matrices to a file
void writeTestFile(int dims, MPI_Comm* world, int worldSize, int myRank) {
    Matrix test = default_matrix;

    test.rows = test.cols = dims;
    int Varray[worldSize];
    int disp[worldSize];
    int i;
    for (i = 0; i < worldSize; i++) {
        Varray[i] = (dims*dims) / worldSize;
    }
    for (i = 0; i < (dims*dims) % worldSize; i++) {
        Varray[i] += 1;
    }
    test.data = (double*)malloc(Varray[myRank]*sizeof(double));
    for (i = 0; i < Varray[myRank]; i++) {
        test.data[i] = (int)rand() % 1;
    }
    int nextLength = 0;
    for (i = 0; i < worldSize; i++) {
        if (i == 0) {
            disp[i] = 0;
            nextLength = Varray[i];
            continue;
        }
        disp[i] = disp[i - 1] + nextLength;
        nextLength = Varray[i];
    }
    MPI_File fh;
    MPI_File_open(*world, "scripttest", MPI_MODE_CREATE | MPI_MODE_WRONLY,
            MPI_INFO_NULL, &fh);
    MPI_Offset off = disp[myRank]*sizeof(double);
    MPI_File_write_at(fh, off, test.data, Varray[myRank], MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);
    free(test.data);
}


void deleteFileTest() {
    if (remove("scripttest") == 0) {
        //puts("Deleted scripttest file successfully");
        return;
    } else {
        //puts("Error in deleting scripttest file");
        return;
    }
}



double* AdjMatrix(int size){
    double* rtn = (double*)malloc(size*size*sizeof(double));
    int i, j;
    for(i=0; i<size*size; i++){
        rtn[i] = rand() % 2;
    }
    for(i=0; i<size; i++){
        for(j=0; j<size; j++){
            if(j == i)
              rtn[size*i + j] = 0;
        }
    }
    return rtn;
}
















int main(int argc, char** argv) {
    srand(time(NULL)); // Seed the random generator

    MPI_Init(&argc, &argv); // Initialize MPI

    MPI_Comm world = MPI_COMM_WORLD; // Get the world

    int worldSize, myRank;
    MPI_Comm_size(world, &worldSize); // Get the world size
    MPI_Comm_rank(world, &myRank); // Get the rank of the "current" node
    //printf("myRank %d\n", myRank);

    // Create the matrices
    Matrix a = default_matrix;
    Matrix Result = default_matrix;
    Matrix Result2 = default_matrix; 

    int i;
    int DIM = 4;
    a.cols = a.rows = DIM;
    Result.cols = Result2.cols = DIM;
    Result.rows = Result2.rows = 1;












    if(myRank == 0){
        a.data = (double*)malloc(a.cols*a.rows*sizeof(double));
        for(i=0; i<a.cols*a.rows; i++){
            a.data[i] = 0;
        }
        a.data[3] = a.data[1] = a.data[2] = 1;
        printMatrix(&a);
    }
    
    Result.data = pageRank(&a, &world, worldSize, myRank);
    if(myRank == 0){
        puts("No file reading: ");
        printMatrix(&Result);
        free(Result.data);
    }
    
    writeTestFileSpecific(DIM, 1, &world, worldSize, myRank);
    
    Result.data = FILEpageRank(DIM, &world, worldSize, myRank);
    if(myRank == 0){
        free(a.data);
        puts("File Reading: ");
        printMatrix(&Result);
        free(Result.data);
    }








    if(myRank == 0){
        a.data = (double*)malloc(a.cols*a.rows*sizeof(double));
        for(i=0; i<a.cols*a.rows; i++){
            a.data[i] = 0;
        }
        a.data[1] = a.data[2] = a.data[3] = a.data[4] = a.data[6] = a.data[8] = a.data[11]  = 1;
        printMatrix(&a);
    }
    
    Result.data = pageRank(&a, &world, worldSize, myRank);
    if(myRank == 0){
        puts("No file reading: ");
        printMatrix(&Result);
        free(Result.data);
    }
    
    writeTestFileSpecific(DIM, 0, &world, worldSize, myRank);
    
    Result.data = FILEpageRank(DIM, &world, worldSize, myRank);
    if(myRank == 0){
        free(a.data);
        puts("File Reading: ");
        printMatrix(&Result);
        free(Result.data);
    }







    if(myRank == 0){
        a.data = (double*)malloc(a.cols*a.rows*sizeof(double));
        for(i=0; i<a.cols*a.rows; i++){
            a.data[i] = 0;
        }
        a.data[1] = a.data[2] = a.data[3] = a.data[7] = a.data[11] = 1;
        printMatrix(&a);
    }
    
    Result.data = pageRank(&a, &world, worldSize, myRank);
    if(myRank == 0){
        puts("No file reading: ");
        printMatrix(&Result);
        free(Result.data);
    }
    
    writeTestFileSpecific(DIM, 0, &world, worldSize, myRank);
    
    Result.data = FILEpageRank(DIM, &world, worldSize, myRank);
    if(myRank == 0){
        free(a.data);
        puts("File Reading: ");
        printMatrix(&Result);
        free(Result.data);
    }









   


    MPI_Finalize(); // Wrap everything up
    return 0;
}
