#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<math.h>
#include<limits.h> //MAX_INT
//#include "BigInt-mast/BigInt.h"

//16576

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME

typedef unsigned long long boi;

static boi lim = 4294967295;

boi overflow(boi a, boi b, int mod){
    //puts("overflew");
    if (a>lim || b>lim) {
        puts("Nothing should have overflew");
        boi k = a/lim;
        boi rem = a%lim;
        printf("k %llu, rem %llu, a %llu, b %llu\n", k, rem, a, b);
        return (overflow(overflow(b,k, mod), lim, mod) + overflow(b, rem, mod));
    }
    boi rtn = a*b;
    return rtn;
}



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
    boi compare_fact = 1;

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

       
    boi i, stuff;
    //puts("BEfor for loop");
    printf("Rank: %d | Start: %llu | Finish: %llu\n", Rank, start, finish);
    //start %= 100000;
    //finish %= 100000;

    //while(start % 10 == 0)
    //  start /= 10;

    //while(finish % 10 == 0)
    //  finish /= 10;
    
    //printf("Rank: %d | Start: %llu | Finish: %llu\n", Rank, start, finish);
    for(i=start; i<=finish; i++){
        stuff = i;

        while(stuff % 10 == 0)
            stuff /= 10;
        stuff %= 100000;

        //local_fact = overflow(local_fact, i, 100000);
        local_fact *= stuff;
        //  printf("local_fact *= %zu: %zu\n",i,local_fact);
        while(local_fact % 10 == 0)
            local_fact /= 10;
        local_fact %= 100000;
        //    puts("local_fact >= 100000");
       
        //if(i%200000 == 0){
        //    printf("%zu\n", i);
        //}
    }
    puts("after first for loop");
    for(i=start; i<=finish; i++){
      compare_fact *= i;  
      /*while(compare_fact % 10 == 0)
          compare_fact /= 10;
      compare_fact %= 100000;*/
      //compare_fact = overflow(compare_fact, i, 100000);       
    }
    puts("after for loop number 2");
    while(compare_fact % 10 == 0)
      compare_fact /= 10;
    compare_fact %= 100000;
    puts("test");
    if(compare_fact != local_fact)
      puts("REEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n");
      printf("  compare_fact: %llu | local_fact %llu\n\n", compare_fact, local_fact);
    //puts("ENd for loop");
    
    
   
    //MPI_PROD overflows so we need to split up the multiplication into steps. We either need to "hack" reduce 
    //function so ranks call it in certian situations (Figure out how many multiplications can be together without overflow)
    //
    //OR
    //
    //us MPI_send and MPI_rec to send local_arr to other nodes and make my own "reduce"
    //printf("local_fact: %zu\n", local_fact);
    //collector[Rank] = local_fact; 
    //puts("Before gather");
    printf("local_fact: %llu\n", local_fact);
    MPI_Gather(&local_fact, 1, MPI_UNSIGNED_LONG_LONG, collector, 1, MPI_UNSIGNED_LONG_LONG, 0, *world);
    
    //puts("After gather");


    if(Rank == 0){
        int w, q;
        //puts("Before unpacking");
       

        for(w=2; w<(2*worldSize); w*=2){
            //printf("Loop2 start w:%d\n", w);
            for(q=0; q<worldSize - w/2; q+=w){
               
                //printf("Loop3 start w:%d q:%d\n", w, q);
                //printf("Loop2 start w:%d q:%d j:%d\n", w, q, j);
                collector[q] *= collector[q+w/2];
                
                //collector[q] = overflow(collector[q], collector[q+w/2], 100000);
                //printf("Collector[j]: %zu\n", collector[j]); 
                while(collector[q] % 10 == 0){
                    collector[q] /= 10;
                    //puts("oh no");
                }
                collector[q] %= 100000;
                
            }
        }
       /* for(k=0; k<worldSize; k++)
            printf("%zu ", collector[k]);
        puts("");*/
        rtn = collector[0]; 
    
    
    //MPI_Reduce(&local_fact, &rtn, 1, MPI_UNSIGNED_LONG_LONG, MPI_PROD, 0, *world);
    //puts("Reduced");
    
    

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
    //MPI_Get_processor_name(processorName, &processSize); 
    srand(time(0));                           // Gives the processor name of current node

    boi fac; 
    //fac = 1000000000000;
    //fac = 20; 
    fac = 99998; 
    //puts("Start factorial!!");
    
    
    
    
    unsigned long long result = factorial(fac, &world, worldSize, myRank);
    //size_t results = factorialTest(fac, &world, worldSize, myRank);
    if(myRank == 0){
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
    }
    MPI_Finalize(); // Finalizing MPI

    return 0;
}
