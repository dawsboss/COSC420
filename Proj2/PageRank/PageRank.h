#ifndef PAGERANK_H_
#define PAGERANK_H_
#include<stdio.h> // I/O
#include<stdlib.h> // Standard library
#include<math.h> // Math functions 
#include<mpi.h> // MPI library
#include"MatrixJawn/matrix.h"//Matrix operations and class

// References the actual index of the 1d array
#define INDEX(A,i,j) A->cols*i + j
// Accesses the actual array using the INDEX macro
#define ACCESS(A,i,j) A->data[INDEX(A,i,j)]


// FUNCTIONS

double* pageRank(Matrix*, MPI_Comm*, int, int);
double* HITS(Matrix*, int, MPI_Comm*, int, int);
double* FILEpageRank(int, MPI_Comm*, int, int);//TODO - Make work on more than one node and multiples
double* FILE_HITS(int, int, MPI_Comm*, int, int);//TODO Broken - outline but no conrete concepts

#endif // _PAGERANK_H_
