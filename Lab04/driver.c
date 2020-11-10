#include<stdio.h> // I/O
#include<stdlib.h> // Standard library functions
#include<mpi.h> // MPI Library
#include<time.h> // Time functions
#include"matrix.h" // Matrix Library

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
        test.data[i] = 1 + rand() % 10;
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

int main(int argc, char** argv){
    srand(time(NULL));

    MPI_Init(&argc, &argv);

    MPI_Comm world = MPI_COMM_WORLD;

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize);
    MPI_Comm_rank(world, &myRank);
    
    if(myRank == 0)
        printf("Start of porgram! | World Size: %d\n", worldSize);    
    
    int testcases[13] = {100, 250, 500, 750, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000};
    int w, ww, dimnum;
   
    dimnum = 13;

    double numofTrials = 10.0; 
        
    double startAdd, finishAdd;
    //double startSub, finishSub;
    //double startMul, finishMul;
    //double startInn, finishInn;
    
    int rowLength;
    int colLength;
        
    Matrix Result=default_matrix;

    int DIMENSION; // Change the test case matrix dimensions 

/*
    for(w=0; w<5; w++){
        DIMENSION = 5;

        printf("count of trials: %d\n", w);
        
        rowLength = DIMENSION;

        Result.rows = rowLength;
        Result.cols = 1;
        writeTestFile(DIMENSION, &world, worldSize, myRank);
        MPI_Barrier(world);
        Result.data = EigenVector("scripttest", DIMENSION, &world, worldSize, myRank);
    }*/

    for(w=0; w<dimnum; w++){
        DIMENSION = testcases[w]; // Change the test case matrix dimensions 
        
        //Testing addition
        
        rowLength = DIMENSION;
        colLength = DIMENSION;
        
        //start timing
        startAdd = MPI_Wtime();

        for(ww=0; ww<numofTrials; ww++){
            Result.rows = rowLength;
            Result.cols = 1;
            writeTestFile(DIMENSION, &world, worldSize, myRank);
            MPI_Barrier(world);
            Result.data = EigenVector("scripttest", DIMENSION, &world, worldSize, myRank);
            if (myRank == 0) {
                deleteFileTest();
            }
            if(myRank == 0){
                //puts("Done");   
                free(Result.data);
                Result = default_matrix;
            }
        }
    
        finishAdd = MPI_Wtime();
        if(myRank == 0)
            printf("EIGENVECTOR -- numofItems: %d | Time/Node: %f \n", testcases[w], finishAdd - startAdd);
        fflush(stdout);
    }   
    
    
    
    for(w=0; w<dimnum; w++){
        DIMENSION = testcases[w]; // Change the test case matrix dimensions 
        
        //Testing addition
        
        rowLength = DIMENSION;
        colLength = DIMENSION;
        
        //start timing
        startAdd = MPI_Wtime();

        for(ww=0; ww<numofTrials; ww++){
            Result.rows = rowLength;
            Result.cols = 1;
            writeTestFile(DIMENSION, &world, worldSize, myRank);
            MPI_Barrier(world);
            Result.data = EigenVectorFile("scripttest", DIMENSION, &world, worldSize, myRank);
            if (myRank == 0) {
                deleteFileTest();
            }
            if(myRank == 0){
                //puts("Done");   
                free(Result.data);
                Result = default_matrix;
            }
        }
    
        finishAdd = MPI_Wtime();
        if(myRank == 0)
            printf("EIGENVECTOR FILE -- numofItems: %d | Time/Node: %f \n", testcases[w], finishAdd - startAdd);
        fflush(stdout);
    }
    
    
    
     
    free(Result.data); 
    if(myRank == 0)
        printf("End of porgram! | World Size: %d\n", worldSize);    
    MPI_Finalize();
    return 0;
    
}
