#ifndef MATRIX_H_
#define MATRIX_H_
#include<stdio.h> // I/O
#include<stdlib.h> // Standard library
#include<math.h> // Math functions 
#include<mpi.h> // MPI library

// References the actual index of the 1d array
#define INDEX(A,i,j) A->cols*i + j
// Accesses the actual array using the INDEX macro
#define ACCESS(A,i,j) A->data[INDEX(A,i,j)]

// Matrix struct definition
struct matrix {
    int rows, cols; // dimensions
    double* data; // pointer to the data, a 1d array
};

// Shortcut for creating a matrix
typedef struct matrix Matrix;

// FUNCTIONS

// Initializes the Matrix
void initMatrix(Matrix*, int, int);
// Prints the Matrix to the screen
void printMatrix(Matrix*);
// Addition of matrices
double* addMatrices(Matrix*, Matrix*, MPI_Comm*, int, int);
// Subtraction of matrices
double* subtractMatrices(Matrix*, Matrix*, MPI_Comm*, int, int);
// Multiplication of matrices
double* multMatrices(Matrix*, Matrix*, MPI_Comm*, int, int);
// Transpose of matrix
double* transpose(Matrix*);
// Inner Product of two matrices
double innerProduct(Matrix*, Matrix*, MPI_Comm*, int, int);

#endif // _MATRIX_H_
