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
        //printf("a: | rows: %d | cols: %d\n", a->rows, a->cols);
        //printMatrix(a);
        //printf("p: | rows: %d | cols: %d\n", p->rows, p->cols);
        //printMatrix(p);
        //printf("oldP: | rows: %d | cols: %d\n", oldP->rows, oldP->cols);
        //printMatrix(oldP);
        //printf("ones: | rows: %d | cols: %d\n", ones->rows, ones->cols);
        //printMatrix(ones);
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

    int j;
    int SuM = 0;
    //puts("unit Length A");
    if(myRank == 0){
        for(i=0; i<a->rows*a->cols; i+=a->cols){     
            for(j=0; j<a->cols; j++){
                SuM += a->data[i+j];
            }
            if(SuM == 0)
                SuM = 1;
            for(j=0; j<a->cols; j++){
                a->data[i+j] /= SuM;
            }
        }   
        puts("WHile loop");
    }
    while(done>=1 && counter<2500){
        free(oldP->data);
        oldP->data = p->data;
        
        Mp.data = multMatrices(a, p, world, worldSize, myRank);

        if(myRank == 0){
            //puts("Mp: ");
            //printMatrix(&Mp);
        }

        p->data = multMatrixConst(&Mp, alpha, world, worldSize, myRank);

        if(myRank == 0){
            free(Mp.data);
            //puts("p->data after mult:");
            //printMatrix(p);
        }
        
        tmp = addMatrices(p, ones, world, worldSize, myRank);
        if(myRank == 0){
            free(p->data);
        }
        p->data = tmp;
        
        if(myRank == 0){
            //puts("p->data after add One:");
            //printMatrix(p);
        }


        if(myRank == 0){
            length = 0;
            for(i=0; i<p->rows; i++){
                length += p->data[i];
            }
        }
        
        
        MPI_Bcast(&length, 1, MPI_DOUBLE, 0, *world);
        
        //if(myRank == 0){
           //printf("Length p:%f\n", length);
        //}
        
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
        if(myRank == 0){
            //puts("P");
            //printMatrix(p);
        }
        //printf("Rank: %d | SendCount[0]: %d | SendCount[1]: %d | disp[0]: %d | disp[1]: %d\n", myRank, sendCount[0], sendCount[1], disp[0], disp[1]);
        //Scatterv
        double buffer[sendCount[myRank]];
        MPI_Scatterv(p->data, sendCount, disp, MPI_DOUBLE, buffer, sendCount[myRank], MPI_DOUBLE, 0, *world);
        
        for(i=0; i<sendCount[myRank]; i++){
            //printf("Rank: %d | buffer[i]: %f\n", myRank, buffer[i]);
            buffer[i] /= length;
        } 

        //Gatherv
        MPI_Gatherv(buffer, sendCount[myRank], MPI_DOUBLE, p->data, sendCount, disp, MPI_DOUBLE, 0, *world);

        if(myRank == 0){
            //puts("p after scatter: ");
            //printMatrix(p);
        }
        
        difference = subtractMatrices(p, oldP, world, worldSize, myRank);
        if(myRank == 0){ 
            puts("pi old");
            printMatrix(p);
            puts("");
            printMatrix(oldP);
        }
        if(myRank == 0){
            /*puts("difference");
            for(i=0; i<4; i++){
                printf("%f \n", difference[i]);
            }*/
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























double* FILEpageRank(int dim, MPI_Comm* world, int worldSize, int myRank){ 
    
    Matrix tes = default_matrix;
    tes.rows = tes.cols = dim; 
    tes.data = (double*)malloc(dim*dim*sizeof(double));

    MPI_File fh; 
  
    MPI_File_open(*world, "scripttest", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
       
    MPI_File_read(fh, tes.data, dim*dim, MPI_DOUBLE, MPI_STATUS_IGNORE);
        
    MPI_File_close(&fh);


    puts("tes read in: =====================================");
    printMatrix(&tes);
        

    free(tes.data);
  
  
  
  
  
  
  
  
  
  
  
    double alpha = .88;//Chance of teleporting between .85 and .9
    int i, masterI;
    int grabbed = 1;//Home many rows of M am I grabbing at a time?



    //Splitting up adjacancy matrix to be read in
    int myRowsCount[worldSize];
    int myRowsDisp[worldSize];

    for(i=0; i<worldSize; i++){
        myRowsCount[i] = dim/worldSize;
    }
    for(i=0; i<(dim%worldSize); i++){
        myRowsCount[i] += 1;
    }

    int sum = 0;
    for(i=0; i<worldSize; i++){
        myRowsDisp[i] = sum;
        sum += myRowsCount[i];
    }

    


   //Init needed varriables for algorithm
    Matrix locPs = default_matrix;
    Matrix* local_ps = &locPs;
    local_ps->rows = myRowsCount[myRank];
    local_ps->cols = 1;
    local_ps->data = (double*)malloc(local_ps->rows * local_ps->cols * sizeof(double));
    for(i=0; i<local_ps->rows * local_ps->cols; i++){
        local_ps->data[i] = 1;
    }
    
    Matrix locP = default_matrix;
    Matrix* local_p = &locP;
    local_p->rows = dim;
    local_p->cols = 1;
    local_p->data = (double*)malloc(local_p->rows * local_p->cols * sizeof(double));
    for(i=0; i<local_p->rows * local_p->cols; i++){
        local_p->data[i] = 1;
    }
    
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
    ones->rows = grabbed;
    ones->cols = 1;
    ones->data = (double*)malloc(ones->cols * ones->rows * sizeof(double));
    for(i=0; i<ones->rows*ones->cols; i++){
        ones->data[i] = (1-alpha);
    }
    if(myRank==0){
        for(i=0; i<p->rows*p->cols; i++){
            p->data[i] = 1;
            oldP->data[i] = 1;
        }
        //printf("p: | rows: %d | cols: %d\n", p->rows, p->cols);
        //printMatrix(p);
        //printf("oldP: | rows: %d | cols: %d\n", oldP->rows, oldP->cols);
        //printMatrix(oldP);
        //printf("ones: | rows: %d | cols: %d\n", ones->rows, ones->cols);
        //printMatrix(ones);
    }


    //Keep doing the same operation until P converges
    double errorTolerance = .0000000000000001;
    double* difference;
    int counter = 0;
    int done = 1;
    
    
    double length;
    
    Matrix Mp = default_matrix;
    Mp.cols = grabbed;
    Mp.rows = 1;

    double* tmp=NULL;


    //tmp = transpose(a);
    //free(a->data);
    int j;
    int SuM = 0;
    //puts("unit Length A");
   






    //puts("WHile loop");
    while(done>=1 && counter<2500){

        //printf("count %d\n", counter);

        free(oldP->data);
        oldP->data = p->data;
         if(myRank == 0){
            //puts("oldP:");
            //printMatrix(p);
        }
        for(masterI=0; masterI<myRowsCount[myRank]; masterI+=grabbed){
            if(myRank == 0){           
                for(i=0; i<p->rows*p->cols; i++){
                    local_p->data[i] = p->data[i]; 
                }
            }
            MPI_Bcast(local_p->data, p->rows*p->cols, MPI_DOUBLE, 0, *world);
        //CREATE A HERE TODO
            //TODO Read in from file M matrix What to read in from above
            Matrix inA = default_matrix;
            Matrix* a = &inA;
            a->cols = dim;
            a->rows = grabbed;
            a->data = (double*)malloc(a->cols*a->rows*sizeof(double));
                //TODO READ
            //int myRowsCount[worldSize];
        //int myRowsDisp[worldSize];
        if(myRank == 0){
             //printf("Rank: %d | myRowsCount: %d | myRowsDisp: %d\n", myRank,myRowsCount[myRank], myRowsDisp[myRank]);    
        }

        MPI_File_open(*world, "scripttest", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
        
        MPI_File_read_at(fh, (masterI + myRowsDisp[myRank])*sizeof(double)*dim , a->data, grabbed*dim, MPI_DOUBLE, MPI_STATUS_IGNORE);
        
        MPI_File_close(&fh);
        
        if(myRank == 0){
            //puts("This should be a read in row");
            //printMatrix(a);
        } 
            


 
            for(i=0; i<a->rows*a->cols; i+=a->cols){     
                SuM = 0;
                //for(j=0; j<a->cols; j++){
                    //printf("%f ", a->data[i+j]);
                //}
                //puts("");
                for(j=0; j<a->cols; j++){
                    //printf("SuM: %d\n", SuM);
                    SuM += a->data[i+j];
                }
                if(SuM == 0)
                    SuM = 1;
                for(j=0; j<a->cols; j++){
                    a->data[i+j] /= SuM;
                }
                //printMatrix(a);
            }      
        
        //puts("a, p");
        //printMatrix(a);
        //printf("\n");
        //printMatrix(local_p);


        Mp.data = multMatricesSerial(a, local_p);
        if(myRank == 0){
           //puts("Mp: ");
           //printMatrix(&Mp);
        }

        tmp = multMatrixConstSerial(&Mp, alpha);
        free(Mp.data);
        Mp.data = tmp;
        
        tmp = addMatricesSerial(&Mp, ones);
        free(Mp.data);
        Mp.data = tmp;
        
        if(myRank == 1){
            //puts("11111111111111111111111 MP");
            //printMatrix(&Mp);
        }
        
        for(i=0; i<grabbed; i++){
            //printf("Rank: %d | i: %d | masterI: %d \n", myRank, i, masterI);
            //printMatrix(local_ps);
            local_ps->data[masterI+i] = Mp.data[i];
            //printf("Mp given to local_ps: %f\n", local_ps->data[masterI+i]);
            //printMatrix(&Mp);
        }
        free(tmp);
        free(a->data);
        }




        length = 0;
        for(i=0; i<local_ps->rows*local_ps->cols; i++){
            length += local_ps->data[i];
        }
        
        if(myRank == 0){
          //printf("Length p:%f\n", length);
        }
        
        for(i=0; i<local_ps->rows*local_ps->cols; i++){
             local_ps->data[i] /= length;
        }

        //puts("local_ps ending ----");
        //printMatrix(local_ps); 

        if(myRank == 0){
            p->data = (double*)malloc(p->rows*p->cols*sizeof(double));
        }
        MPI_Gatherv(local_ps->data, myRowsCount[myRank], MPI_DOUBLE, p->data, myRowsCount, myRowsDisp, MPI_DOUBLE, 0, *world);
        if(myRank == 0){
            //puts("p oldp");
            //printMatrix(p);
            //puts("");
            //printMatrix(oldP);
        }


        //NOW all data is back on root node
        difference = subtractMatrices(p, oldP, world, worldSize, myRank);
    
       
        
        
        int sendCount[worldSize];
        int disp[worldSize];
        
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

    free(local_p->data);
    free(local_ps->data);
    free(ones->data);
    if(myRank == 0){
        printf("count %d\n", counter);
        free(oldP->data);
        return p->data;
    }
    return NULL;
    


}





//hubOrNo means if you want a score hub to be returned or the  score
double* HITS(Matrix* ta, int HubOrAuth, MPI_Comm* world, int worldSize, int myRank){
    int i;

    Matrix a_ = default_matrix;
    copyMatrix(&a_, ta);
    Matrix* a = &a_;

    Matrix at_ = default_matrix;
    Matrix* aT = &at_;
    aT->rows = a->rows;
    aT->cols = a->cols;
    if(myRank == 0)
        aT->data = transpose(a);

    Matrix use_ = default_matrix;
    Matrix* use = &use_;
    use->rows = ta->rows;
    use->cols = ta->cols;


    if(HubOrAuth <= 0){
        use->data = multMatrices(a, aT, world, worldSize, myRank);
    }else{
        use->data = multMatrices(aT, a, world, worldSize, myRank);
    }


    Matrix x_ = default_matrix;
    Matrix old_ = default_matrix;

    Matrix* x = &x_;
    Matrix* old = &old_;
    x->rows = old->rows = a->rows;
    x->cols = old->cols = 1;

    x->data = NULL;
    old->data = NULL;

    if(myRank == 0){
        x->data = (double*)malloc(x->rows*x->cols*sizeof(double));
        old->data = (double*)malloc(old->rows*old->cols*sizeof(double));
        for(i=0; i<x->cols*x->rows; i++){
            x->data[i] = old->data[i] = 1;
        }
    }


    int count = 0;
    double length;
    double* difference;
    double errorTol = 0.0000000000000001; //~10^-16
    int done = 0;

    while(count<10000 && done == 0){
        free(old->data);
        old->data = x->data;
        x->data = multMatrices(use, x, world, worldSize, myRank); 
        
        length = L2Norm(x, world, worldSize, myRank);
        MPI_Bcast(&length, 1, MPI_DOUBLE, 0, *world);

        int sendCount[worldSize];
        int disp[worldSize];

        for(i=0; i<worldSize; i++){
            sendCount[i] = (x->rows*x->cols)/worldSize;
        }
        for(i=0; i<(x->cols*x->rows)%worldSize; i++){
            sendCount[i] += 1;
        }
        int sum = 0;
        for(i=0; i<worldSize; i++){
            disp[i] = sum;
            sum += sendCount[i];
        }

        double buffer[sendCount[myRank]];

        MPI_Scatterv(x->data, sendCount, disp, MPI_DOUBLE, buffer, sendCount[myRank], MPI_DOUBLE, 0, *world);


        for(i=0; i<sendCount[myRank]; i++){
            buffer[i] /= length;
        }

 
        MPI_Gatherv(buffer, sendCount[myRank], MPI_DOUBLE, x->data, sendCount, disp, MPI_DOUBLE, 0, *world);
        
        difference = subtractMatrices(x, old, world, worldSize, myRank);

        if(myRank == 0){
            done = 1;
            for(i=0; i<x->cols*x->rows; i++){
                if((difference[i]>0 ? difference[i] : difference[i]*-1) > errorTol){
                    done = 1;
                    break;
                }
            }
        }
        free(difference);
        MPI_Bcast(&done, 1, MPI_DOUBLE, 0, *world);
        count++;
    }
  
    if(myRank == 0){
        free(old->data);
        free(use->data);
        free(aT->data);
        free(a->data);    
        return x->data;
    }
    return NULL;

}






















//Big Boken
double* FILE_HITS(int dim, int HubOrAuth, MPI_Comm* world, int worldSize, int myRank){ 
    
    Matrix tes = default_matrix;
    tes.rows = tes.cols = dim; 
    tes.data = (double*)malloc(dim*dim*sizeof(double));

    MPI_File fh; 
  
    MPI_File_open(*world, "scripttest", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
       
    MPI_File_read(fh, tes.data, dim*dim, MPI_DOUBLE, MPI_STATUS_IGNORE);
        
    MPI_File_close(&fh);


    puts("tes read in: =====================================");
    printMatrix(&tes);
        

    free(tes.data);
 

  
  
    int i, masterI;
    int grabbed = 1;//Home many rows of M am I grabbing at a time?



    //Splitting up adjacancy matrix to be read in
    int myRowsCount[worldSize];
    int myRowsDisp[worldSize];

    for(i=0; i<worldSize; i++){
        myRowsCount[i] = dim/worldSize;
    }
    for(i=0; i<(dim%worldSize); i++){
        myRowsCount[i] += 1;
    }

    int sum = 0;
    for(i=0; i<worldSize; i++){
        myRowsDisp[i] = sum;
        sum += myRowsCount[i];
    }



    //Create the USE matrix for algoerithm use
    for(masterI=0; masterI<myRowsCount[myRank]; masterI+=grabbed){
        Matrix inA = default_matrix;
        Matrix* a = &inA;
        a->cols = dim;
        a->rows = grabbed;
        a->data = (double*)malloc(a->cols*a->rows*sizeof(double));

            
        MPI_File_open(*world, "scripttest", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
        
        MPI_File_read_at(fh, (masterI + myRowsDisp[myRank])*sizeof(double)*dim , a->data, grabbed*dim, MPI_DOUBLE, MPI_STATUS_IGNORE);
        
        MPI_File_close(&fh);
        
        Matrix aT = default_matrix;
        aT.cols = grabbed;
        aT.rows = dim;
        aT.data = transpose(a);
        
        
        puts("A");
        printMatrix(a);
        puts("aT");
        printMatrix(&aT);
        
        
        
        
        double* Temp;
        if(HubOrAuth <= 0){
            Temp = multMatrices(a, &aT, world, worldSize, myRank);
        }else{
            printf("aT: r: %d | c: %d || a: r: %d | c%d\n", aT.rows, aT.cols, a->rows, a->cols);
            Temp = multMatrices(&aT, a, world, worldSize, myRank);
        }

        puts("Temp");
        int jj;
        for(jj = 0; jj<a->rows*aT.cols; jj++){
            printf("%f ", Temp[jj]);
        }


        MPI_File_open(*world, "UseVector", MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fh);
        MPI_Offset off = myRowsDisp[myRank]*dim*sizeof(double);
        MPI_File_write_at(fh, off, Temp, myRowsCount[myRank], MPI_DOUBLE, MPI_STATUS_IGNORE);
        free(Temp);   
    } 











    MPI_File_open(*world, "UseVector", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
       
    MPI_File_read(fh, tes.data, dim*dim, MPI_DOUBLE, MPI_STATUS_IGNORE);
        
    MPI_File_close(&fh);


    puts("USE read in: =====================================");
    printMatrix(&tes);
        

    free(tes.data);
 







   //Init needed varriables for algorithm
    Matrix locPs = default_matrix;
    Matrix* local_ps = &locPs;
    local_ps->rows = myRowsCount[myRank];
    local_ps->cols = 1;
    local_ps->data = (double*)malloc(local_ps->rows * local_ps->cols * sizeof(double));
    for(i=0; i<local_ps->rows * local_ps->cols; i++){
        local_ps->data[i] = 1;
    }
    
    Matrix locP = default_matrix;
    Matrix* local_p = &locP;
    local_p->rows = dim;
    local_p->cols = 1;
    local_p->data = (double*)malloc(local_p->rows * local_p->cols * sizeof(double));
    for(i=0; i<local_p->rows * local_p->cols; i++){
        local_p->data[i] = 1;
    }
    
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
    
    
    if(myRank==0){
        for(i=0; i<p->rows*p->cols; i++){
            p->data[i] = 1;
            oldP->data[i] = 1;
        }
        //printf("p: | rows: %d | cols: %d\n", p->rows, p->cols);
        //printMatrix(p);
        //printf("oldP: | rows: %d | cols: %d\n", oldP->rows, oldP->cols);
        //printMatrix(oldP);
        //printf("ones: | rows: %d | cols: %d\n", ones->rows, ones->cols);
        //printMatrix(ones);
    }


    //Keep doing the same operation until P converges
    double errorTolerance = .0000000000000001;
    double* difference;
    int counter = 0;
    int done = 1;
    
    
    double length;
    
    Matrix Mp = default_matrix;
    Mp.cols = grabbed;
    Mp.rows = 1;
    
    Matrix use = default_matrix;
    use.cols = grabbed;
    use.rows = 1;
    
   



    double* tmp=NULL;


    int j;
    int SuM = 0;
   






    //puts("WHile loop");
    while(done>=1 && counter<2500){

        //printf("count %d\n", counter);

        free(oldP->data);
        oldP->data = p->data;
        
        for(masterI=0; masterI<myRowsCount[myRank]; masterI+=grabbed){
            if(myRank == 0){           
                for(i=0; i<p->rows*p->cols; i++){
                    local_p->data[i] = p->data[i]; 
                }
            }
            MPI_Bcast(local_p->data, p->rows*p->cols, MPI_DOUBLE, 0, *world);

            use.data = (double*)malloc(use.cols*use.rows*sizeof(double));
            
            MPI_File_open(*world, "UseVector", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
        
            MPI_File_read_at(fh, (masterI + myRowsDisp[myRank])*sizeof(double)*dim , use.data, grabbed*dim, MPI_DOUBLE, MPI_STATUS_IGNORE);
        
            MPI_File_close(&fh);
        

            
            
 
        
        Mp.data = multMatricesSerial(&use, local_p);

        
        for(i=0; i<grabbed; i++){
            local_ps->data[masterI+i] = Mp.data[i];
        }
        free(tmp);
        free(use.data);
        }




        length = L2NormSerial(local_ps);
        
        for(i=0; i<local_ps->rows*local_ps->cols; i++){
             local_ps->data[i] /= length;
        }


        if(myRank == 0){
            p->data = (double*)malloc(p->rows*p->cols*sizeof(double));
        }
        MPI_Gatherv(local_ps->data, myRowsCount[myRank], MPI_DOUBLE, p->data, myRowsCount, myRowsDisp, MPI_DOUBLE, 0, *world);




        //NOW all data is back on root node
        difference = subtractMatrices(p, oldP, world, worldSize, myRank);
              
        int sendCount[worldSize];
        int disp[worldSize];
        
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

    free(local_p->data);
    free(local_ps->data);
    if(myRank == 0){
        printf("count %d\n", counter);
        free(oldP->data);
        return p->data;
    }
    return NULL;
    


}



