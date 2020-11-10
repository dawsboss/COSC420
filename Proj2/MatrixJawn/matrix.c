#include"matrix.h"

static Matrix default_matrix = {0,0,0};
/*
 *Through out thei progrma we used the cache size of the machine
 *  we worked on to make the code more effiecient
 *Our L1 cashe size was : 32k
 *  Found using cat /sys/devices/system/cpu/cpu0/cache/index1/size
 */
static int cashe = 32000;

// Initializes a "Matrix" struct with random values
// MUST have rows and columns initialized or will seg fault
// USE ON ONE NODE
void initMatrix(Matrix *a, int rows, int cols) {
    if(a->data)
      free(a->data);
  
    int i, j;
    a->rows = rows;
    a->cols = cols;
    a->data = (double*)malloc(a->rows*a->cols*sizeof(double));
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            ACCESS(a,i,j) = rand() % 10 + 1;
        }
    }
}

// Sets a's meta data and data equal to b' with a deep copy
// USE ON ONE NODE OR THERE WILL BE OVERLAP
void copyMatrix(Matrix* a, Matrix* b){
    //printf("b : rows %d x cols %d",b->rows, b->cols);
  
    if(a->data)
      free(a->data);
    
    a->rows = b->rows;
    a->cols = b->cols;

    if(b->data){
        a->data = (double*) malloc(b->rows*b->cols*sizeof(double));
 
        int z;
        for(z=0; z<(b->cols*b->rows); z++){
            a->data[z] = b->data[z];
        }
    }
}

void initMatrixIdentity(Matrix* a, int rows, int cols){
    if(a->data)
      free(a->data);

    a->rows = rows;
    a->cols = cols;

    a->data = (double*) malloc(rows*cols*sizeof(double));
    int i,j;
    for(i=0; i<rows;i++){
        for(j=0;j<cols;j++){
            if(i==j){
                a->data[INDEX(a,i,j)] = 1;
                continue;
            }
            a->data[INDEX(a,i,j)] = 0;
        }
    }       
}


// Prints the Matrix that is passed to it
// USE ON ONE NODE OR THERE WILL BE OVERLAP
void printMatrix(Matrix *a) {
    int i, j;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < a->cols; j++) {
            printf("%0.2f ", ACCESS(a,i,j));
        }
        puts("");
    }
    puts("");
}

// Adds two matrices together
double* addMatrices(Matrix *a, Matrix *b, MPI_Comm *world, int worldSize, int myRank) {
    if (a->rows != b->rows && a->cols != b->cols) {
        puts("Wrong dimensions");
        return NULL;
    }
    //We need to utilize all the nodes given by adding the scatterv and gatherv so that 
    //we can make certainn nodes get more stuff to do than other nodes
    //
    //ex. 6 elements and 5 nodes means one node needs to do 1 extra comutation.
    //If we used regular scatter and gather the last one extra would be ignored and not 
    //calculated. So we used scatterV and gatherV so we can give the extra to nodes
    //But these functions need an array of how many elements should go places  
    int length = a->rows * a->cols;
    int Varray[worldSize];
    int displacement[worldSize];
    int j;
    // Initialize Varray sizes
    for(j=0; j<(worldSize); j++){
    Varray[j] = length / worldSize;
    }
    // Pick up any stragglers
    for(j=0; j<(length % worldSize); j++){
    Varray[j] += 1;
    }
    // Initialize displacement array using Varray values
    int nextLength = 0;
    for (j = 0; j < worldSize; j++) {
    if (j == 0){
        displacement[j] = 0;
        nextLength = Varray[j];
        continue;
    }
    displacement[j] = displacement[j - 1] + nextLength;
    nextLength = Varray[j];
    }

    int matLen = Varray[myRank]; // Each nodes divied up array sizes
    double* rtn = NULL; // For the root node to initialize
    if(myRank == 0 ) // Root node will initialize the return array
    rtn = (double*) malloc(length*sizeof(double));

    // Each local node solution
    double* local_solution = (double*) malloc(matLen*sizeof(double));
    // Local matrix for A
    double* local_matA = (double*) malloc(matLen*sizeof(double));
    // Local matrix for B
    double* local_matB = (double*) malloc(matLen*sizeof(double));
    MPI_Scatterv(a->data, Varray, displacement, MPI_DOUBLE, local_matA, matLen, MPI_DOUBLE, 0, *world);
    MPI_Scatterv(b->data, Varray, displacement, MPI_DOUBLE, local_matB, matLen, MPI_DOUBLE, 0, *world);
    // Each now has their needed a data and b data now to add them
    int i;
    int matLenByte = matLen*sizeof(double);
    int cash = (matLenByte/cashe);
    if(matLenByte<cashe){
        for(i=0; i<matLen; i++)
           local_solution[i] = local_matA[i] + local_matB[i];
    }else{
        for(i=0; i<matLenByte*cash; i+=cash){
          for(j=i; j<i+cash && j<matLen; j++)
              local_solution[i] = local_matA[i] + local_matB[i]; 
        }
    }
    // Gather all of the solutions back
    MPI_Gatherv(local_solution, matLen, MPI_DOUBLE, rtn, Varray, displacement, MPI_DOUBLE, 0, *world);

    free(local_solution);
    free(local_matA);
    free(local_matB);
    return rtn;
}

// Subtracts two matrices from each other
double* subtractMatrices(Matrix *a, Matrix *b, MPI_Comm *world, int worldSize, int myRank) {
    if (a->rows != b->rows && a->cols != b->cols) {
        puts("Wrong dimensions");
        return NULL;
    }
    int length = a->rows * a->cols;
    int Varray[worldSize];
    int displacement[worldSize];
    int j;
    // Initialize Varray sizes
    for(j=0; j<(worldSize); j++){
    Varray[j] = length / worldSize;
    }
    // Pick up any stragglers
    for(j=0; j<(length % worldSize); j++){
    Varray[j] += 1;
    }
    // Initialize displacement array using Varray values
    int nextLength = 0;
    for (j = 0; j < worldSize; j++) {
    if (j == 0){
        displacement[j] = 0;
        nextLength = Varray[j];
        continue;
    }
    displacement[j] = displacement[j - 1] + nextLength;
    nextLength = Varray[j];
    }

    int matLen = Varray[myRank]; // Each nodes divied up array sizes
    double* rtn = NULL; // For the root node to initialize
    if(myRank == 0 ) // Root node will initialize the return array
    rtn = (double*) malloc(length*sizeof(double));

    // Each local node solution
    double* local_solution = (double*) malloc(matLen*sizeof(double));
    // Local matrix for A
    double* local_matA = (double*) malloc(matLen*sizeof(double));
    // Local matrix for B
    double* local_matB = (double*) malloc(matLen*sizeof(double));
    MPI_Scatterv(a->data, Varray, displacement, MPI_DOUBLE, local_matA, matLen, MPI_DOUBLE, 0, *world);
    MPI_Scatterv(b->data, Varray, displacement, MPI_DOUBLE, local_matB, matLen, MPI_DOUBLE, 0, *world);
    // Each now has their needed a data and b data now to add them 
    int i;
    int matLenByte = matLen*sizeof(double);
    int cash = (matLenByte/cashe);
    if(matLenByte<cashe){
        for(i=0; i<matLen; i++)
           local_solution[i] = local_matA[i] - local_matB[i];
    }else{
        for(i=0; i<matLenByte*cash; i+=cash){
          for(j=i; j<i+cash && j<matLen; j++)
              local_solution[i] = local_matA[i] - local_matB[i]; 
        }
    }
    // Gather all of the solutions back
    MPI_Gatherv(local_solution, matLen, MPI_DOUBLE, rtn, Varray, displacement, MPI_DOUBLE, 0, *world);

    free(local_solution);
    free(local_matA);
    free(local_matB);
    return rtn;
}

// Subtract matrices in serial
double* subtractMatricesSerial(Matrix* a, Matrix* b) {
    if (a->rows != b->rows && a->cols != b->cols) {
        puts("Wrong dimensions");
        return NULL;
    }
    //printf("a rows: %d a cols: %d\n", a->rows, a->cols);
    //printf("b rows: %d b cols: %d\n", b->rows, b->cols);

    double* rtnMe = (double*)malloc(a->rows*a->cols*sizeof(double));
    int i;
    for (i = 0; i < a->rows*a->cols; i++) {
        rtnMe[i] = a->data[i] - b->data[i];
    }
    return rtnMe;
}

// Does matrix multiplication 
double* multMatrices(Matrix *a, Matrix *b, MPI_Comm *world, int worldSize, int myRank) {
    if (a->cols != b->rows) {
        puts("WRONG, dimensions do not match matrix multiplication formula");
        return NULL;
    }
    
    double* rtn = NULL;
    if (myRank == 0) {
        rtn = (double*)malloc(a->rows*b->cols*sizeof(double));
    }
    Matrix atmp = default_matrix; // Holds the vector of current row of A
    Matrix btmp = default_matrix; // Holds the vector of current column of B

    initMatrix(&atmp, 1, a->cols);
    initMatrix(&btmp, b->rows, 1);
    // Start the process
    int i, j, l;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            // Copy over the current row of A
            if (myRank == 0) {
                for (l = 0; l < a->cols; l++) {
                    atmp.data[l] = ACCESS(a,i,l);
                }
            }
            // Copy over the current row of B
            if (myRank == 0) {
                for (l = 0; l < b->rows; l++) {
                    btmp.data[l] = ACCESS(b,l,j);
                }
            }
            double innerProd = innerProduct(&atmp, &btmp, world, worldSize, myRank);
            if (myRank == 0) {
                rtn[INDEX(b,i,j)] = innerProd;
            }
        }
    }

    // Free the tmp arrays before we leave
    free(atmp.data);
    free(btmp.data);

    // Return the result
    if(myRank == 0){
        //int ree;
        /*for(ree = 0; ree<(a->rows*b->cols); ree++){
            printf("%f ",rtn[ree]);
        }*/
       

        //printMatrix(a);
        //printMatrix(b);
    }
    return rtn;
}

// Multiply matrices in serial
double* multMatricesSerial(Matrix* a, Matrix* b) {
    if (a->cols != b->rows) {
        puts("WRONG, dimensions do not match matrix multiplication formula");
        return NULL;
    }

    double* rtnMe = (double*)malloc(a->rows*b->cols*sizeof(double));
    int i, j, l;
    double sum;
    for (i = 0; i < a->rows; i++) {
        for (j = 0; j < b->cols; j++) {
            sum = 0; 
            for (l = 0; l < a->cols; l++) {
                sum += ACCESS(a,i,l) * ACCESS(b,l,j);
            }
            rtnMe[b->cols*i + j] = sum;
        }
    }
    return rtnMe;
}

// Transposes the given matrix
// CAN ONLY BE USED BY ONE NODE
// Returns a pointer to an array that is the transposed matrix
// Matrix that is accepting this argument must be transposed dimensionally already
double* transpose(Matrix* a) {
    // Create the new matrix with the dimensions of the passed matrix

    double *rtn = (double*)malloc(a->rows*a->cols*sizeof(double));

    // Swap i and j for every element in the array
    int i, j;
    for (i = 0; i < a->cols; i++) {
        for (j = 0; j < a->rows; j++) {
            rtn[a->rows*i + j] = ACCESS(a,j,i);
        }
    }
    // Return the pointer to the array
    return rtn;
}

double* multMatrixConst(Matrix* a, double b, MPI_Comm *world, int worldSize, int myRank){ 
        
    int sendCount[worldSize];
    int disp[worldSize];
    int i;
    
    double* rtn = NULL;
    if(myRank == 0)
        rtn = (double*)malloc(a->cols* a->rows * sizeof(double));

    for(i=0; i<worldSize; i++){
        sendCount[i] = (a->cols*a->rows)/worldSize;
    }
    for(i=0; i<(a->cols*a->rows)%worldSize; i++){
        sendCount[i] += 1;
    }
    int sum = 0;
    for(i=0; i<worldSize; i++){
        disp[i] = sum;
        sum += sendCount[i];
    }

    double buffer[sendCount[myRank]];
                
    MPI_Scatterv(a->data, sendCount, disp, MPI_DOUBLE, buffer, sendCount[myRank], MPI_DOUBLE, 0, *world);
       
        
    for(i=0; i<sendCount[myRank]; i++){
        buffer[i] = buffer[i]*b;      
    }

    MPI_Gatherv(buffer, sendCount[myRank], MPI_DOUBLE, rtn, sendCount, disp, MPI_DOUBLE, 0, *world);
    if(myRank == 0)
        return rtn;
    return NULL;
}

// Calculates the inner product of two matrices
// MUST ENTER COLUMN VECTORS!!!!
double innerProduct(Matrix *a, Matrix *b, MPI_Comm *world, int worldSize, int myRank) {
    if (a->rows != 1 && a->cols != 1) {
        puts("A matrix is not a vector");
        return -31999;
    }
    if (b->rows != 1 && b->cols != 1) {
        puts("B matrix is not a vector");
        return -31999;
    }
    if (a->rows*a->cols != b->rows*b->cols) {
        puts("Matrices are not the same length");
        return -31999;
    }
     
    int length = a->rows * a->cols;
    int Varray[worldSize];
    int displacement[worldSize];
    int j;
    // Initialize Varray sizes
    for(j=0; j<(worldSize); j++){
        Varray[j] = length / worldSize;
    }
    // Pick up any stragglers
    for(j=0; j<(length % worldSize); j++){
        Varray[j] += 1;
    }
    // Initialize displacement array using Varray values
    int nextLength = 0;
    for (j = 0; j < worldSize; j++) {
        if (j == 0){
            displacement[j] = 0;
            nextLength = Varray[j];
            continue;
        }
        displacement[j] = displacement[j - 1] + nextLength;
        nextLength = Varray[j];
    }
  
    int matLen = Varray[myRank]; // Each nodes divied up array sizes
    double rtnResult = 0; // Final answer
    
    // Local matrix for A
    double* local_matA = (double*) malloc(matLen*sizeof(double));
    // Local matrix for B
    double* local_matB = (double*) malloc(matLen*sizeof(double));
    MPI_Scatterv(a->data, Varray, displacement, MPI_DOUBLE, local_matA, matLen, MPI_DOUBLE, 0, *world);
    MPI_Scatterv(b->data, Varray, displacement, MPI_DOUBLE, local_matB, matLen, MPI_DOUBLE, 0, *world);

    double sum = 0; 
    int i;
    int matLenByte = matLen*sizeof(double);
    int cash = (matLenByte/cashe);
    if(matLenByte<cashe){
        for(i=0; i<matLen; i++)
           sum += local_matA[i] * local_matB[i];
    }else{
        for(i=0; i<matLenByte*cash; i+=cash){
          for(j=i; j<i+cash && j<matLen; j++)
              sum += local_matA[i] * local_matB[i]; 
        }
    }

    // Sum the remaining sums
    MPI_Reduce(&sum, &rtnResult, 1, MPI_DOUBLE, MPI_SUM, 0, *world);

    free(local_matA);
    free(local_matB);
    if (myRank == 0) {
        return rtnResult;
    }
    return -1;
} 

//  Gauss-Jordan Inverse algorithm only works for square matriciesies where 
//  A * A.T = I
double* GaussJordan(Matrix* at, Matrix* bt, MPI_Comm* world, int worldSize, int myRank){
    //Setup copies of ta and tb so we don't corrupt them
    Matrix tmpa = default_matrix;
    Matrix tmpb = default_matrix;
    Matrix* a=&tmpa;
    Matrix* b=&tmpb;

    copyMatrix(a, at);
    copyMatrix(b, bt);




  
    // Check to make sure the matrices form a valid linear system
    if (a->rows != b->rows) {
        printf("Rank: %d got yeeted | a->rows: %d a->cols %d b->rows %d b->cols %d\n", myRank, a->rows, a->cols, b->rows, b->cols);
        return NULL;
    }
    int k,i,r,c,j;

    int Varray[worldSize];
    int disp[worldSize];
    int Varray2[worldSize];
    int disp2[worldSize];

    // Set up Varray and displacement for scattering of rows to scale (A matrix)
    for (i = 0; i < worldSize; i++) {
        Varray[i] = (a->rows / worldSize) * a->cols;
    }
    for (i = 0; i < (a->rows % worldSize); i++) {
        Varray[i] += a->cols;
    }
    int nextLength = 0;
    for (i = 0; i < worldSize; i++) {
        if (i == 0){
            disp[i] = 0;
            nextLength = Varray[i];
            continue;
        }
        disp[i] = disp[i - 1] + nextLength;
        nextLength = Varray[i];
    }
    // Set up Varray and displacement for scattering of rows to scale (B matrix)
    for (i = 0; i < worldSize; i++) {
        Varray2[i] = (b->rows / worldSize) * b->cols;
    }
    for (i = 0; i < (b->rows % worldSize); i++) {
        Varray2[i] += b->cols;
    }
    nextLength = 0;
    for (i = 0; i < worldSize; i++) {
        if (i == 0){
            disp2[i] = 0;
            nextLength = Varray2[i];
            continue;
        }
        disp2[i] = disp2[i - 1] + nextLength;
        nextLength = Varray2[i];
    }
    
    // Recv buffer for elements of rows that each node will compute with
    double* local_row_mat = (double*)malloc(Varray[myRank]*sizeof(double));
    double* local_b_mat = (double*)malloc(Varray2[myRank]*sizeof(double));

    // Individual l vector
    double l[a->rows];  
    // a's Piviot row
    double ak[a->rows];
    // b's Piviot row
    double bk[b->rows];

    for(k=0; k<a->rows; k++){
        // Compute the vector scalings Li = Ai,k/Ak,k for all i
        if (myRank == 0) {
            for(i=0; i<a->rows; i++){//compute l[k,i]
                l[i] = ACCESS(a,i,k)/ACCESS(a,k,k); 
            }
            for(c=0; c<a->cols; c++)
                ak[c] = ACCESS(a,k,c);
            for(c=0; c<b->cols; c++)
                bk[c] = ACCESS(b,k,c);
        }
        // Scatter the rows of A that each node will apply l vector to
        MPI_Scatterv(a->data, Varray, disp, MPI_DOUBLE, local_row_mat, Varray[myRank], MPI_DOUBLE, 0, *world);
        // Scatter the rows of B that each node will apply l vector to
        MPI_Scatterv(b->data, Varray2, disp2, MPI_DOUBLE, local_b_mat, Varray2[myRank], MPI_DOUBLE, 0, *world);
        // Scatter the value of the l vector
        MPI_Bcast(&l, a->rows, MPI_DOUBLE, 0, *world);
        //Broadcast the a's kth row
        MPI_Bcast(&ak, a->rows, MPI_DOUBLE, 0, *world);
        //Broadcast the b's kth row 
        MPI_Bcast(&bk, b->rows, MPI_DOUBLE, 0, *world);

        // Perform the following on n nodes

        int offsetK = disp[myRank]/a->cols;
        for(r=0; r<(Varray[myRank] / a->cols); r++){    
            if (k == r+offsetK) {
                continue;
            }
            for(c=0; c<a->cols; c++){  
                local_row_mat[INDEX(a,r,c)] = local_row_mat[INDEX(a,r,c)] - (l[r+offsetK] * ak[c]);
            }
            for(c=0; c<b->cols; c++){
                local_b_mat[INDEX(b,r,c)] = local_b_mat[INDEX(b,r,c)] - (l[r+offsetK] * bk[c]);
            }
        }

        
            // Free the data from each matrix so that new ones can be assigned
        if(myRank == 0){
            free(a->data);
            free(b->data);
            a->data = (double*)malloc(a->rows*a->cols*sizeof(double));
            b->data = (double*)malloc(b->rows*b->cols*sizeof(double));
        }


        // Gather the rows of A back from each node
        MPI_Gatherv(local_row_mat, Varray[myRank], MPI_DOUBLE, a->data, Varray, disp, MPI_DOUBLE, 0, *world);
        // Gather the rows of B back from each node
        MPI_Gatherv(local_b_mat, Varray2[myRank], MPI_DOUBLE, b->data, Varray2, disp2, MPI_DOUBLE, 0, *world);
    }
    if (myRank == 0) {
        // Create the scalar vector that contains the diagonal elements of a
        double ll[a->cols];
        for(i=0;i<a->rows; i++){
            for(j=0; j<a->cols; j++){
                if(i==j){
                    ll[i] = ACCESS(a,i,j);   
                }
            }
        }
        // Scale A by the final scalar vector
        for(i=0; i<a->rows; i++){
            for(j=0; j<a->cols; j++){
                ACCESS(a,i,j) = ACCESS(a,i,j) / ll[i];
            }
        }   
        // Scale b by the final scalar vector
        for(i=0; i<b->rows; i++){
            for(j=0; j<b->cols; j++){
                ACCESS(b,i,j) = ACCESS(b,i,j) / ll[i];
            }
        }   
    }

    free(a->data);
    // Free the local row matrix

    free(local_row_mat);
    free(local_b_mat);
    if(myRank == 0){
        return b->data;
    }
    return NULL;
}


double L2Norm(Matrix* a, MPI_Comm* world, int worldSize, int myRank){
    if(a->rows > 1 && a->cols != 1){
        printf("Given matrix must be a vector");
        return -1;
    }
    if(a->cols > 1 && a->rows != 1){
        printf("Given matrix must be a vector");
        return -1;
    }
     
    if(a->rows ==1 ){
        //puts("Transpose a->rows");
        double* tmp = transpose(a);
        free(a->data);
        a->data = tmp;
    }
    //printf("a: | a.cols: %d | a.rows: %d\n", a->cols, a->rows);
    /*int w, t;
    for(w=0; w<a->rows; w++){
        for(t=0; t<a->cols; t++){
            printf("%f ", ACCESS(a,w, t));
        }
        puts("");
    }
    puts("");
    */
    int i;
    double TotalSum = 0;
    double local_sum = 0;
    int sendCount[worldSize];
    int disp[worldSize];
    for(i=0; i<worldSize; i++){
        sendCount[i] = (a->cols*a->rows)/worldSize;
    }
    for(i=0; i<(a->cols*a->rows)%worldSize; i++){
        sendCount[i] += 1;
    }
    int sum = 0;
    for(i=0; i<worldSize; i++){
        disp[i] = sum;
        sum += sendCount[i];
    }

    double buffer[sendCount[myRank]];
    local_sum = 0; 
    //printf("Rank: %d | local_sum: %f\n", myRank, local_sum);
    MPI_Scatterv(a->data, sendCount, disp, MPI_DOUBLE, buffer, sendCount[myRank], MPI_DOUBLE, 0, *world);
    
    //printf("Rank: %d | local_sum: %f\n", myRank, local_sum);
    for(i=0; i<sendCount[myRank]; i++){
        //printf("Rank: %d | local_sum: %f | buffer: %f\n", myRank, local_sum, buffer[i]);
        local_sum += pow(buffer[i], 2);
        //printf("Rank: %d | local_sum: %f\n", myRank, local_sum);
    }

    printf("Rank: %d | local_sum: %f\n", myRank, local_sum);

    MPI_Reduce(&local_sum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, *world);
    
    if(TotalSum==0)
        return 1.0;

    return sqrt(TotalSum);
}

// L2Norm in serial
double L2NormSerial(Matrix* a) {
    if((a->rows > 1 && a->cols != 1) || (a->cols > 1 && a->rows != 1)){
        printf("Given matrix must be a vector");
        return -1;
    }
     
    if(a->rows == 1){
        double* tmp = transpose(a);
        free(a->data);
        a->data = tmp;
    }

    int i;
    double sum = 0;
    for (i = 0; i < a->rows; i++) {
        sum += pow(a->data[i], 2); 
    }
    
    return sqrt(sum);
}


double* EigenVector(char* filename, int dims, MPI_Comm* world, int worldSize, int myRank){
    // Create the a matrix
    MPI_File fh;
    Matrix* a;
    Matrix test = default_matrix;
    a = &test;
    a->rows = a->cols = dims;
    if (myRank == 0) {
        a->data = (double*)malloc(dims*dims*sizeof(double)); // make sure to free
    }

    MPI_File_open(*world, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    if (myRank == 0) {
        MPI_File_read(fh, test.data, dims*dims, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }
    MPI_File_close(&fh);
    if (myRank == 0) {
        puts("Matrix in file");
        printMatrix(a);     
    }
    
    Matrix x = default_matrix;
    Matrix oldx = default_matrix;

    x.rows = oldx.rows  = a->rows;
    x.cols = oldx.cols = 1;
    int i;
    double* data = NULL;
    double* data2 = NULL;
    if(myRank == 0){
        data = (double*)malloc(x.cols*x.rows*sizeof(double)); 
        data2 = (double*)malloc(x.cols*x.rows*sizeof(double));
        for(i=0; i<x.rows*x.cols; i++){
            data[i] = 1;
            data2[i] = 1;
        }
    }
    x.data = data;
    oldx.data = data2;
    int count = 0;
    double length;
    double* difference;
    double errorTolerance = 0.0000000000000001;//pow(10,-16);
    int done = 0; 

    while(count<10000 && done==0){
       // printf("%d\n",count);
        free(oldx.data);
        oldx.data = x.data;
        x.data = multMatrices(a, &x, world, worldSize, myRank);
         
        //if(myRank == 0)
        
        
        length = L2Norm(&x, world, worldSize, myRank);
         
        MPI_Bcast(&length, 1, MPI_DOUBLE, 0, *world);
        
        int sendCount[worldSize];
        int disp[worldSize];

        for(i=0; i<worldSize; i++){
            sendCount[i] = (x.cols*x.rows)/worldSize;
        }
        for(i=0; i<(x.cols*x.rows)%worldSize; i++){
            sendCount[i] += 1;
        }
        int sum = 0;
        for(i=0; i<worldSize; i++){
            disp[i] = sum;
            sum += sendCount[i];
        }

        double buffer[sendCount[myRank]];
                
        MPI_Scatterv(x.data, sendCount, disp, MPI_DOUBLE, buffer, sendCount[myRank], MPI_DOUBLE, 0, *world);
        
        
        for(i=0; i<sendCount[myRank]; i++){
            buffer[i] /= length;      
        }

        MPI_Gatherv(buffer, sendCount[myRank], MPI_DOUBLE, x.data, sendCount, disp, MPI_DOUBLE, 0, *world);
        
  
        
        difference = subtractMatrices(&x, &oldx, world, worldSize, myRank);
        
        if(myRank == 0){
            /*printf("Error Tolerance: %e\n", errorTolerance);
            puts("Difference:");
            for(i=0; i<x.cols*x.rows; i++)
                printf("%e ",difference[i]);
            puts("");*/
            done = 1;
            for(i=0; i<x.cols*x.rows; i++){
                //printf("abs(diff): ");
                if((difference[i]>0 ? difference[i] : difference[i]*-1) > errorTolerance){
                    done = 0;
                    break;
                }
            }
        }
        if(myRank==0)
            free(difference);
        
        MPI_Bcast(&done, 1, MPI_DOUBLE, 0, *world);

        count++;
        //printf("done: %d\n", done);
    }
    if(myRank == 0){
        printf("Ending count: %d\n\n",count);
        free(oldx.data);
        free(a->data);
        return x.data;
    }
    return NULL;
}

// EigenVectorFile Function:
// Calculates the eigenvector of a given matrix using mpi file operations
// NOTE: Matrix in file must coincide with dimensions given or function will break
double* EigenVectorFile(int dims, MPI_Comm* world, int worldSize, int myRank) {
    // Files evvector and evmatrix are precomputed before calling this function***
    
    Matrix a = default_matrix;
    Matrix x = default_matrix;
    Matrix oldx = default_matrix;

    // Set the dimensions of the x vector
    x.rows = oldx.rows = a.cols = dims;
    x.cols = oldx.cols = 1;
    int i;
    // Each node malloc for the x vector (replacing broadcast)
    double* data = (double*)malloc(x.rows*x.cols*sizeof(double));
    double* data2 = (double*)malloc(x.rows*x.cols*sizeof(double));
    MPI_File fh;
    MPI_File_open(*world, "evvector", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_read(fh, data, dims, MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);
    MPI_File_open(*world, "evvector", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_read(fh, data2, dims, MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    x.data = data;
    oldx.data = data2;

    // Each node malloc their "chunk" of a
    int Varray[worldSize];
    int disp[worldSize];
    for (i = 0; i < worldSize; i++) {
        Varray[i] = (dims / worldSize) * dims;
    }
    for (i = 0; i < (dims % worldSize); i++) {
        Varray[i] += dims;
    }
    /*
    int nextLength = 0;
    for (i = 0; i < worldSize; i++) {
        if (i == 0){
            disp[i] = 0;
            nextLength = Varray[i];
            continue;
        }
        disp[i] = disp[i - 1] + nextLength;
        nextLength = Varray[i];
    }
    */
    // Local array buffer to read in individual "chunk of a"
    double *local_a = (double*)malloc(Varray[myRank]*sizeof(double));
    MPI_Offset off = myRank*Varray[myRank]*sizeof(double);
    MPI_File_open(*world, "evmatrix", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_read_at_all(fh, off, local_a, Varray[myRank], MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);
    // Assign to a.data
    a.data = local_a;
    a.rows = Varray[myRank] / dims;

    int count = 0;
    double length;
    double* difference = NULL;
    double errorTolerance = 0.0000000000000001;//pow(10,-16);
    int done = 0; 

    if (a.rows == 0) {
        printf("myRank is %d and I am useless for this\n", myRank);
        return NULL;
    }
    puts("A");
    printMatrix(&a);
    puts("x");
    printMatrix(&x);
    MPI_Barrier(*world);
    while(count<10000 && done==0){
        free(oldx.data);
        oldx.data = x.data;
        x.data = multMatricesSerial(&a, &x); // Mult matrices can't be done in parallel here
        x.rows = Varray[myRank] / dims;
        ///*
        if (myRank == 0) {
            printMatrix(&x);
        }
        MPI_Barrier(*world);
        if (myRank == 1) {
            printMatrix(&x);
        }
        MPI_Barrier(*world);
        //*/
         
        length = L2NormSerial(&x); // L2Norm can't be done in parallel here
        ///*
        if (myRank == 0) {
            printf("length %f\n", length);
        }
        MPI_Barrier(*world);
        if (myRank == 1) {
            printf("length %f\n", length);
        }
        MPI_Barrier(*world);
        //*/

        for (i = 0; i < Varray[myRank] / dims; i++) {
            x.data[i] /= length;
        }
        ///*
        //printMatrix(&x);
        if (myRank == 0) {
            printMatrix(&x);
        }
        MPI_Barrier(*world);
        if (myRank == 1) {
            printMatrix(&x);
        }
        MPI_Barrier(*world);
        //*/
        Matrix myOldx = default_matrix;
        myOldx.rows = Varray[myRank] / dims;
        myOldx.cols = 1;
        myOldx.data = (double*)malloc(myOldx.rows*myOldx.cols*sizeof(double));
        MPI_File_open(*world, "disperseoldx", MPI_MODE_CREATE | MPI_MODE_WRONLY,
                MPI_INFO_NULL, &fh);
        if (myRank == 0) {
            MPI_File_write(fh, oldx.data, dims, MPI_DOUBLE, MPI_STATUS_IGNORE);
        }
        MPI_File_close(&fh);
        MPI_Barrier(*world);
        MPI_File_open(*world, "disperseoldx", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
        off = myRank*(Varray[myRank]/dims)*sizeof(double);
        MPI_File_read_at_all(fh, off, myOldx.data, Varray[myRank] / dims, MPI_DOUBLE, MPI_STATUS_IGNORE);
        MPI_File_close(&fh);

        
        /*
        if (myRank == 0 ) {
            printMatrix(&myOldx); 
        }
        MPI_Barrier(*world);
        if (myRank == 1) {
            printMatrix(&myOldx);
        }
        */
        //oldx.rows = Varray[myRank] / dims;
        difference = subtractMatricesSerial(&x, &myOldx);
        //oldx.rows = dims;
        free(myOldx.data); // free the copy of oldx

        /*
        if (myRank == 0) {
            puts("After subtraction");
            for(i = 0; i < x.rows; i++) {
                printf("element %d = %f\n", i, difference[i]);
            }
        }
        MPI_Barrier(*world);
        if (myRank == 1) {
            puts("After subtraction");
            for(i = 0; i < x.rows; i++) {
                printf("element %d = %f\n", i, difference[i]);
            }
        }
        MPI_Barrier(*world);
        */
        /*
        done = 1;
        for(i=0; i<Varray[myRank] / dims; i++){
            //printf("abs(diff): ");
            if((difference[i]>0 ? difference[i] : difference[i]*-1) > errorTolerance){
                done = 0;
                break;
            }

        }
        */
        //puts("After matrix subtraction");
        MPI_File_open(*world, "outputvector", MPI_MODE_CREATE | MPI_MODE_WRONLY,
                MPI_INFO_NULL, &fh);
        off = myRank*(Varray[myRank]/dims)*sizeof(double);
        MPI_File_write_at(fh, off, x.data, Varray[myRank]/dims, MPI_DOUBLE, MPI_STATUS_IGNORE);
        MPI_File_close(&fh);
        MPI_Barrier(*world); // Must wait till both writes are done
        MPI_File_open(*world, "outputvector", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
        free(x.data);
        // Flip back
        x.rows = dims;
        x.data = (double*)malloc(x.rows*x.cols*sizeof(double));
        MPI_File_read(fh, x.data, dims, MPI_DOUBLE, MPI_STATUS_IGNORE);
        MPI_File_close(&fh);
        MPI_Barrier(*world);
        /*
        if (myRank == 0) {
            printMatrix(&x);
        }
        MPI_Barrier(*world);
        if (myRank == 1) {
            printMatrix(&x);
        }
        MPI_Barrier(*world);
        */

        free(difference);

        count++;
    }
    puts("outside loop");
    if (myRank == 0) {
        puts("Node 1");
        printMatrix(&x);
    }
    MPI_Barrier(*world);
    if (myRank == 1) {
        puts("Node 2");
        printMatrix(&x); 
    }
    MPI_Barrier(*world);
    return NULL;
    /*
    // Write each nodes chunks to a file
    MPI_File_open(*world, "outputvector", MPI_MODE_CREATE | MPI_MODE_WRONLY,
            MPI_INFO_NULL, &fh);
    off = myRank*(Varray[myRank]/dims)*sizeof(double);
    MPI_File_write_at(fh, off, x.data, Varray[myRank]/dims, MPI_DOUBLE, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);
    // Read in to the root
    x.data = NULL;
    if (myRank == 0) {
        MPI_File_open(*world, "outputvector", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
        x.data = (double*)malloc(x.rows*x.cols*sizeof(double));
        MPI_File_read(fh, x.data, x.rows*x.cols, MPI_DOUBLE, MPI_STATUS_IGNORE);
        MPI_File_close(&fh);
    }
*/
    puts("eigenvector:");
    printMatrix(&x);
    return x.data;
    /*
    if(myRank == 0)
        free(oldx.data);
    return x.data;
    */
}
