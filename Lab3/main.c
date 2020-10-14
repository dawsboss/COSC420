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
    //printf("myRank %d\n", myRank);

    // Create the matrices
    Matrix a;
    Matrix b;
    // For addition result
    Matrix c;
    // For subtraction result
    Matrix d;
    // For transpose
    Matrix e;
    // For inner product
    Matrix f;
    Matrix g;
    // For matrix multiplication
    Matrix h;
    Matrix i;
    // Result for matrix multiplication
    Matrix j;
    // Matrix test for Gauss Jordan function
    Matrix gj;
    // Matrix b for Gauss Jordan function
    Matrix gjb;
    // Result matrix for Gauss Jordan function
    Matrix gjresult;

    // The row and column length for matrix a, b, and c
    int rowLength =  2, colLength = 3;
    if (myRank == 0) {
        puts("Matrix a for addition and subtraction");
        initMatrix(&a, rowLength, colLength); // Initialize it
        printMatrix(&a); // Print the array after initialization

        puts("Matrix b for addition and subtraction");
        initMatrix(&b, rowLength, colLength);
        printMatrix(&b);
    
        initMatrix(&c, rowLength, colLength);
        free(c.data); // We change the result of c
        initMatrix(&d, rowLength, colLength);
        free(d.data); // We change the result of d
        initMatrix(&e, rowLength, colLength);
        puts("Matrix to be transposed");
        printMatrix(&e);

        initMatrix(&f, 3, 1);
        initMatrix(&g, 3, 1);
        puts("f matrix for inner product:");
        printMatrix(&f);
        puts("g matrix for inner product:");
        printMatrix(&g);

        initMatrix(&h, 4, 4);
        initMatrix(&i, 4, 4);
        puts("h matrix for matrix multiplication:");
        printMatrix(&h);
        puts("i matrix for matrix multiplication:");
        printMatrix(&i);
        initMatrix(&j, 4, 4);
        free(j.data); // We change the result of j

        initMatrix(&gj, 3, 3);
        puts("Matrix a for gauss jordan function");
        printMatrix(&gj);
        initMatrix(&gjb, 3, 1);
        puts("Matrix b for gauss jordan function");
        printMatrix(&gjb);
        initMatrix(&gjresult, 3, 1);
        free(gjresult.data); // We change the result of gjresult
    } else {
        // So other nodes know the dimensions
        a.rows = b.rows = c.rows = rowLength;
        a.cols = b.cols = c.cols = colLength;
        f.rows = g.rows = 3;
        f.cols = g.cols = 1;
        h.rows = i.rows = j.rows = 4;
        h.cols = i.cols = j.cols = 4;
        gj.rows = gjresult.rows = gj.cols = 3;
        gjresult.cols = 1;
    }
   



    //Testing opperations
    
    //The root must pass the actual data while the workers do not care 
    //so they are seperated
    c.data = addMatrices(&a, &b, &world, worldSize, myRank);
    if(myRank == 0) {
        puts("Result Matrix: a + b"); 
        printMatrix(&c);
    }

    d.data = subtractMatrices(&a, &b, &world, worldSize, myRank);
    if(myRank == 0){
      puts("Result Matrix: a - b");
      printMatrix(&d);
    }

    if (myRank == 0) {
        puts("Transposed Matrix:");
        Matrix tmp;
        initMatrix(&tmp, colLength, rowLength);
        free(tmp.data);
        tmp.data = transpose(&e);
        printMatrix(&tmp);
        free(tmp.data);
        free(e.data);
    }

    double innerProdResult = innerProduct(&f, &g, &world, worldSize, myRank);
    if (myRank == 0) {
        printf("The inner product result is %f\n", innerProdResult);
    }

    j.data = multMatrices(&h, &i, &world, worldSize, myRank);
    if (myRank == 0) {
        puts("Matrix Mult Answer:");
        printMatrix(&j);
    }

    gjresult.data = GaussJordan(&gj, &gjb, &world, worldSize, myRank);
    if (myRank == 0) {
        puts("Result of Gauss Jordan Reduction, linear system answer");
        printMatrix(&gjresult);
    }

    MPI_Finalize(); // Wrap everything up
    // Free the arrays of each matrix
    if (myRank == 0) {
        free(a.data);
        free(b.data); 
        free(gj.data);
        free(gjb.data); // gjresult will point to gjb when pointer returned, watch for double free!!!!
    }
    free(c.data);
    free(d.data);
    free(f.data);
    free(g.data);
    free(h.data);
    free(i.data);
    free(j.data);

    return 0;
}
