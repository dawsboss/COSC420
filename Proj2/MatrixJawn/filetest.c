#include<stdio.h> // I/O
#include<stdlib.h> // Standard library
#include<mpi.h> // MPI library
#include<time.h> // Time functions
#include"matrix.h" // File with matrix definitions and operations

int main(int argc, char** argv) {
    srand(time(NULL)); // Seed the random generator

    MPI_Init(&argc, &argv); // Initialize MPI

    MPI_Comm world = MPI_COMM_WORLD; // Get the world

    int worldSize, myRank;
    MPI_Comm_size(world, &worldSize); // Get the world size
    MPI_Comm_rank(world, &myRank); // Get the rank of the "current" node

    MPI_File fh;
    /*
    MPI_File_open(world, "infile", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

    // Buffer for the file
    double ab[40];

    MPI_File_read(fh, ab, 40, MPI_DOUBLE, MPI_STATUS_IGNORE);

    if (myRank == 0) {
        int i;
        for (i = 0; i < 40; i++) {
            printf("%f ", ab[i]);
        }
    }

    MPI_File_close(&fh);
    int i;
    double ay[20], be[20];
    for (i = 0; i < 20; i++) {
        ay[i] = ab[i];
        be[i] = ab[i + 20];
    }
    */

    // Create the matrices
    Matrix a = default_matrix;
    Matrix b = default_matrix;
    // For addition result
    Matrix c = default_matrix;

    /*
    if (myRank == 0) {
        a.data = ay;
        b.data = be;
    } else {
        a.rows = b.rows = c.rows = 4;
        a.cols = b.cols = c.cols = 5;
    }
    */

    if (myRank == 0) {
        initMatrix(&a, 4, 5);
        printMatrix(&a);
        initMatrix(&b, 4, 5);
        printMatrix(&b);
        initMatrix(&c, 4, 5);
        free(c.data);
    } else {
        a.rows = b.rows = c.rows = 4;
        a.cols = b.cols = c.cols = 5;
    }

    // Test file operations

    MPI_File_open(world, "outfile", MPI_MODE_CREATE | MPI_MODE_WRONLY,
            MPI_INFO_NULL, &fh);
   
    //Testing opperations
    
    //The root must pass the actual data while the workers do not care 
    //so they are seperated
    if (myRank == 0) {
        
    }
    c.data = addMatrices(&a, &b, &world, worldSize, myRank);
    if (myRank == 0) {
        printf("Result for a + b\n");
        printMatrix(&c);
    }
    // For writing to file
    if (myRank != 0) {
        c.data = (double*)malloc(c.rows*c.cols*sizeof(double));
    }
    int Varray[worldSize];
    int disp[worldSize];
    int z;
    for (z = 0; z < worldSize; z++) {
        Varray[z] = (c.rows*c.cols) / worldSize;
    }
    for (z = 0; z < (c.rows*c.cols) % worldSize; z++) {
        Varray[z] += 1;
    }
    int nextLength = 0;
    for (z = 0; z < worldSize; z++) {
        disp[z] = nextLength;
        nextLength += Varray[myRank];
    }
    double local_mat[Varray[myRank]];
    MPI_Scatterv(c.data, Varray, disp, MPI_DOUBLE, local_mat, Varray[myRank], MPI_DOUBLE, 0, world);

    MPI_Offset offset = Varray[myRank] * myRank * sizeof(double);

    MPI_File_write_at(fh, offset, local_mat, Varray[myRank], MPI_DOUBLE, MPI_STATUS_IGNORE);

    if (myRank == 0) {
        printf("Result for a + b\n");
        printMatrix(&c);
    }

    if (myRank == 0) {
        free(a.data);
        free(b.data);
    }
    free(c.data);

    MPI_File_close(&fh);
    MPI_Finalize(); // Wrap everything up
    return 0;
}
