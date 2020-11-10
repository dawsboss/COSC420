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
    Matrix a = default_matrix;
    Matrix b = default_matrix;
    // For addition result
    Matrix c = default_matrix;
    // For subtraction result
    Matrix d = default_matrix;
    // For transpose
    Matrix e = default_matrix;
    // For inner product
    Matrix f = default_matrix;
    Matrix g = default_matrix;
    // For matrix multiplication
    Matrix h = default_matrix;
    Matrix i = default_matrix;
    // Result for matrix multiplication
    Matrix j = default_matrix;
    // Matrix test for Gauss Jordan function
    Matrix gj = default_matrix;
    // Matrix b for Gauss Jordan function
    Matrix gjb = default_matrix;
    // Result matrix for Gauss Jordan function
    Matrix gjresult = default_matrix;
    //Matrix to check if jkresult is correct
    Matrix GJtest = default_matrix;

    //EigenVector testing matrix
    Matrix Eig = default_matrix;
    //EigenVector from Eig
    Matrix EigVec = default_matrix;
    //EigenVector result test
    Matrix EigVecTest = default_matrix;

    // The row and column length for matrix a, b, and c
    int rowLength =  2, colLength = 3;
    if (myRank == 0) {
        initMatrix(&Eig, 4, 4);
        
        initMatrix(&EigVec, 4, 1);
        free(EigVec.data);

        initMatrix(&EigVecTest, 4, 1);
        free(EigVecTest.data);
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
        initMatrix(&GJtest, 3, 1);
        free(GJtest.data);
    
    } else {
        // So other nodes know the dimensions
        a.rows = b.rows = c.rows = rowLength;
        a.cols = b.cols = c.cols = colLength;
        f.rows = g.rows = 3;
        f.cols = g.cols = 1;
        h.rows = i.rows = j.rows = 4;
        h.cols = i.cols = j.cols = 4;
        gj.rows = gjb.rows = gjresult.rows = GJtest.rows = gj.cols = 3;
        gjresult.cols = gjb.cols = GJtest.cols = 1;
        EigVecTest.rows = Eig.rows = Eig.cols = EigVec.rows = 4;
        EigVec.cols = EigVecTest.cols = 1;
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
        Matrix tmp=default_matrix;
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
        GJtest.data = multMatrices(&gj, &gjresult, &world, worldSize, myRank);
   if(myRank == 0){
       printMatrix(&GJtest);
   } 


   //puts("REEEEEEEEEEEEEEEEEEEEEEE NEW JAWN");

 
   Matrix aa = default_matrix;
   Matrix bb = default_matrix;
   Matrix rr = default_matrix;
   Matrix rrtest = default_matrix;
   if(myRank == 0){
       initMatrix(&aa, 4, 4);
       initMatrixIdentity(&bb, 4, 4); 
       initMatrix(&rr, 4, 4);
       initMatrix(&rrtest, 4, 4);
       free(rr.data);
       free(rrtest.data);

   }else{
       rr.rows = rr.cols = aa.rows = aa.cols = bb.rows = bb.cols = 4;          
   }
   if(myRank == 0){
       puts("aa: ");
       printMatrix(&aa);
       puts("bb: ");
       printMatrix(&bb);
   }

   rr.data = GaussJordan(&aa, &bb, &world, worldSize, myRank);
   
   if(myRank == 0){
      puts("GaussJordan Result: ");
      printMatrix(&rr);
   }
   rrtest.data = multMatrices(&aa, &rr, &world, worldSize, myRank);
   
   if(myRank == 0){
      puts("test GSresult: ");
      printMatrix(&rrtest);
    }


   
    //Eigen Vector testing

    /*
    if(myRank == 0){
        printf("Matrix that we are finding the eigen vector for: \n");
        printMatrix(&Eig);
    }
    */
    MPI_File fh;
    MPI_File_open(world, "evmatrix", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    if (myRank == 0) {
        free(Eig.data);
        Eig.data = (double*)malloc(Eig.rows * Eig.cols * sizeof(double));
        MPI_File_read(fh, Eig.data, Eig.rows*Eig.cols, MPI_DOUBLE, MPI_STATUS_IGNORE);
        printf("Matrix that we are finding the eigen vector for: \n");
        printMatrix(&Eig);
    }
    MPI_File_close(&fh);
    EigVec.data = EigenVector("evmatrix", 4, &world, worldSize, myRank);
    
    if(myRank == 0){
        printf("Eigen Vector: \n");
        printMatrix(&EigVec);
    
        //printf("Length of Eig Vector: %f\n", L2Norm(&EigVec, &world, worldSize, myRank));
    }
    
    EigVecTest.data = multMatrices(&Eig, &EigVec, &world, worldSize, myRank);
   
    if(myRank == 0){
        puts("Eig * EigVec = ");
        printMatrix(&EigVecTest); 
    
    
        int w;
    
        for(w=0; w<EigVec.cols*EigVec.rows; w++){
            printf("Ax = x*(Eigen Value) | Eigen Value: %f\n", EigVecTest.data[w]/EigVec.data[w]);
        }
    }
    
    // Eigen Vector File Testing
    // Generating a matrix for the file
    if (myRank == 0)
        puts("Starting EigenVectorFile Function");
    free(EigVec.data);

    EigVec.data = EigenVectorFile("evmatrix", 4, &world, worldSize, myRank);
    if (myRank == 0) {
        printf("Eigen Vector: \n");
        printMatrix(&EigVec);

        //printf("Length of Eig Vector: %f\n", L2Norm(&EigVec, &world, worldSize, myRank));
    }
    free(EigVecTest.data);
    
    EigVecTest.data = multMatrices(&Eig, &EigVec, &world, worldSize, myRank);

    if (myRank == 0) {
        puts("Eig * EigVec = ");
        printMatrix(&EigVecTest);

        int w;

        for(w=0; w<EigVec.cols*EigVec.rows; w++){
             printf("Ax = x*(Eigen Value) | Eigen Value: %f\n", EigVecTest.data[w]/EigVec.data[w]);
        }
    }
    MPI_Finalize(); // Wrap everything up
    // Free the arrays of each matrix
    if (myRank == 0) {
        free(a.data);
        free(b.data); 
        free(gj.data);
        free(gjb.data); // gjresult will point to gjb when pointer returned, watch for double free!!!!
        free(GJtest.data);
        free(aa.data);
        free(bb.data);
        free(rr.data);
        free(rrtest.data);
        free(Eig.data);
    }
    free(EigVec.data);
    free(EigVecTest.data);
    free(gjresult.data);
    free(c.data);
    free(d.data);
    free(f.data);
    free(g.data);
    free(h.data);
    free(i.data);
    free(j.data); 
    return 0;
}
