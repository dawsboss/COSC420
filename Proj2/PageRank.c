#include"PageRank.h"
#include"MatrixJawn/matrix.h"
#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>


double* pageRank(Matrix* ai, MPI_Comm* world, int worldSize, int myRank){
    int dim = ai->rows;
    double alpha = .88;//Chance of teleporting between .85 and .9
    if(dim == 1)
        dim = ai->cols;
    int i;

    //Copping ai to a so we don;t destory the ai matrix
    Matrix inA = default_matrix;
    Matrix* a = &inA;
    a->cols = ai->cols;
    a->rows = ai->rows;
    if(myRank == 0){
        a->data = (double*)malloc(a->cols*a->rows*sizeof(double));
        for(i=0; i<a->cols*a->rows; i++){
            a->data[i] = ai->data[i];
        }
    }
    //Init needed varriables for algorithm
    Matrix pt = default_matrix;
    Matrix* p = &pt;
    p->rows = dim;
    p->cols = 1;
    if(myRank==0)
        p->data = (double*)malloc(p->cols * p->rows * sizeof(double));
    
    Matrix PreviousP = default_matrix;
    Matrix* oldP = &PreviousP;
    oldP->rows = dim;
    oldP->cols = 1;
    if(myRank==0)
        oldP->data = (double*)malloc(oldP->cols * oldP->rows * sizeof(double));
    
    Matrix tOnes = default_matrix;
    Matrix* ones = &tOnes;
    ones->rows = dim;
    ones->cols = 1;
    if(myRank==0)
        ones->data = (double*)malloc(ones->cols * ones->rows * sizeof(double));
    
    if(myRank==0){
        for(i=0; i<p->rows*p->cols; i++){
            p->data[i] = 1;
            oldP->data[i] = 1;
            ones->data[i] = (1-alpha);
        }
        printf("a: | rows: %d | cols: %d\n", a->rows, a->cols);
        printMatrix(a);
        printf("p: | rows: %d | cols: %d\n", p->rows, p->cols);
        printMatrix(p);
        printf("oldP: | rows: %d | cols: %d\n", oldP->rows, oldP->cols);
        printMatrix(oldP);
        printf("ones: | rows: %d | cols: %d\n", ones->rows, ones->cols);
        printMatrix(ones);
    }


    //Keep doing the same operation until P converges
    double errorTolerance = .0000000000000001;
    double* difference;
    int counter = 0;
    int done = 1;

    int sendCount[worldSize];
    int disp[worldSize];
    double length;
    
    Matrix Mp = default_matrix;
    Mp.cols = dim;
    Mp.rows = 1;

    double* tmp=NULL;

    //TODO 'a' needs to be unit lenght in the where the out going links come from .
    //so if the rows rep the out going inks from page i then the rows need
    //to be unit length 

    //tmp = transpose(a);
    //free(a->data);
    int j;
    int SuM = 0;
    //puts("unit Length A");
    if(myRank == 0){
        for(i=0; i<a->rows*a->cols; i+=a->cols){     
            SuM = 0;
            for(j=0; j<a->cols; j++){
                printf("%f ", a->data[i+j]);
            }
            puts("");
            for(j=0; j<a->cols; j++){
                printf("SuM: %d\n", SuM);
                SuM += a->data[i+j];
            }
            if(SuM == 0)
                SuM = 1;
            for(j=0; j<a->cols; j++){
                a->data[i+j] /= SuM;
            }
            printMatrix(a);
        }   
        puts("WHile loop");
    }
    while(done>=1 && counter<10000){
        free(oldP->data);
        oldP->data = p->data;
        //TODO DO PAGE RANK JAWN
        
        if(myRank == 0){
            puts("oldP:");
            printMatrix(oldP);
        }
        Mp.data = multMatrices(a, p, world, worldSize, myRank);

        if(myRank == 0){
            puts("Mp: ");
            printMatrix(&Mp);
        }

        p->data = multMatrixConst(&Mp, alpha, world, worldSize, myRank);

        if(myRank == 0){
            free(Mp.data);
            puts("p->data after mult:");
            printMatrix(p);
        }
        
        tmp = addMatrices(p, ones, world, worldSize, myRank);
        if(myRank == 0){
            free(p->data);
        }
        p->data = tmp;
        
        if(myRank == 0){
            puts("p->data after subtract One:");
            printMatrix(p);
        }


        if(myRank == 0){
            length = 0;
            for(i=0; i<p->rows; i++){
                length += p->data[i];
            }
        }
        
        
        MPI_Bcast(&length, 1, MPI_DOUBLE, 0, *world);
        
        if(myRank == 0)
            printf("Length p:%f\n", length);
        
        
        for(i=0; i<worldSize; i++){
            sendCount[i] = (p->rows * p->cols)/worldSize;
        }
        for(i=0; i<(p->cols*p->rows)%worldSize; i++){
            sendCount[i] += 1;
        }
        int sum = 0;
        for(i=0; i<worldSize; i++){
            disp[i] = sum;
            sum += sendCount[i];
        }

        //Scatterv
        double buffer[sendCount[myRank]];
        MPI_Scatterv(p->data, sendCount, disp, MPI_DOUBLE, buffer, sendCount[myRank], MPI_DOUBLE, 0, *world);
        
        for(i=0; i<sendCount[myRank]; i++){
            buffer[i] /= length;
        } 

        //Gatherv
        MPI_Gatherv(buffer, sendCount[myRank], MPI_DOUBLE, p->data, sendCount, disp, MPI_DOUBLE, 0, *world);

        if(myRank == 0){
            puts("p after scatter: ");
            printMatrix(p);
        }
        
        difference = subtractMatrices(p, oldP, world, worldSize, myRank);
    
        if(myRank == 0){
            puts("difference");
            for(i=0; i<4; i++){
                printf("%f \n", difference[i]);
            }
        }
        //splits up job for error Tolerance
        for(i=0; i<worldSize; i++){
            sendCount[i] = (p->rows*p->cols)/worldSize;
        }
        for(i=0; i<(p->rows*p->cols)%worldSize; i++){
            sendCount[i] += 1;
        }
        sum = 0;
        for(i=0; i<worldSize; i++){
            disp[i] = sum;
            sum += sendCount[i];
        }
        double buffer2[sendCount[myRank]];
        int local_done;
        MPI_Scatterv(difference, sendCount, disp, MPI_DOUBLE, buffer2, sendCount[myRank], MPI_DOUBLE, 0, *world);
        local_done = 0;
        for(i=0; i<sendCount[myRank]; i++){
            if(buffer2[i]>0 ? buffer2[i] : buffer2[i]*-1 > errorTolerance){
                local_done = 1;
                break;
            }
        }
        
        MPI_Reduce(&local_done, &done, 1, MPI_INT, MPI_SUM, 0, *world);
        free(difference);
        MPI_Bcast(&done, 1, MPI_INT, 0,  *world);
        counter++;
    }
    if(myRank == 0){
        printf("count %d\n", counter);
        free(oldP->data);
        free(ones->data);
        free(a->data);
        return p->data;
    }
    return NULL;
    


}


