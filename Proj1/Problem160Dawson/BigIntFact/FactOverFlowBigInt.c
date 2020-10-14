#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<math.h>
#include<limits.h> //MAX_INT
#include"BigInt.h"
//#include "BigDIgits/bigd.h"//More BigDigits stuff
//16576

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME

typedef unsigned long long boi;
typedef int bool;
#define true 1
#define false 0

//watch -n1 squeue
//sbatch P160.sh

boi factorial(boi fact, MPI_Comm *world, int worldSize, int Rank){
    boi* collector = NULL;
    if(Rank == 0)
      collector = (boi*) malloc(worldSize*sizeof(boi));

    //printf("worldSize:%d Rank:%d\n", worldSize, Rank);

    boi start;
    boi finish;
    boi local_fact=1;
    boi rtn=1;

    if(Rank == 0){
        start = (boi)2;
        finish = (boi) (fact/worldSize);
    }else if(Rank == worldSize-1){
         
        start = (boi) (((fact/worldSize) * Rank)+1);
        finish = (boi) fact; 
    }else{
        start = (boi) (((fact/worldSize) * Rank)+1);
        finish = (boi) start + (fact/worldSize) -1;
    }

       
    boi i;
    printf("Rank: %d | Start: %llu | Finish: %llu\n", Rank, start, finish);
    for(i=start; i<=finish; i++){
        //local_fact = overflow(local_fact, i, 100000, true);
    }
    printf("local_fact: %llu\n", local_fact);
    MPI_Gather(&local_fact, 1, MPI_UNSIGNED_LONG_LONG, collector, 1, MPI_UNSIGNED_LONG_LONG, 0, *world);
    

    if(Rank == 0){
        int w, q;
        for(w=2; w<(2*worldSize); w*=2){
            for(q=0; q<worldSize - w/2; q+=w){
               
                //collector[q] = overflow(collector[q], collector[q+w/2], 100000, true);
                while(collector[q] % 10 == 0){
                    collector[q] /= 10;
                    //puts("oh no");
                }
                collector[q] %= 100000;
                
            }
        }
        rtn = collector[0]; 

        if(rtn >= 100000)
          rtn %= 100000;
        free(collector);
        return rtn;
    }
    return -1;
}















int main(int argc, char** argv) {

    MPI_Init(&argc, &argv); // Initializing MPI

    MPI_Comm world = MPI_COMM_WORLD; // Communicator

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize); // Gives amount of nodes running in parallel

    
    MPI_Comm_rank(world, &myRank); // Gives the rank (number) node
    srand(time(0));                           // Gives the processor name of current node

    BigInt* new_big_int = BigInt_construct(42);

    BigInt_print(new_big_int);


    puts("Testing");

    boi result;
    boi fac; 
    fac = 50000; 
    
    BigInt* thing = BigInt_construct(9999999999);//ULLONG_MAX;
    BigInt* two = BigInt_construct(9999999999);//ULLONG_MAX;
    
    BigInt_add(thing, two);
    BigInt_print(thing); 
     
    
    //result = factorial(fac, &world, worldSize, myRank);
    /*if(myRank == 0){
        //printf("Ree %zu \n",results);
        printf("eeR %llu\n", result);
        printf("WORLD SIZE: %d\n", worldSize);
    }
    //M
    //puts("Done factorial!!");
    if(myRank == 0){
        if(result == 16576){
            puts("NUT");
        }else{
            puts("REE");
        }
    }*/
    MPI_Finalize(); // Finalizing MPI

    return 0;
}
