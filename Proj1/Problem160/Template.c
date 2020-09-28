#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<math.h>

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME


//watch -n1 squeue
//sbatch P160.sh


size_t factorial(size_t fact, MPI_Comm *world, int worldSize, int Rank){
    size_t* collector = NULL;
    if(Rank == 0)
      collector = (size_t*) malloc(worldSize*sizeof(size_t));

    //printf("worldSize:%d Rank:%d\n", worldSize, Rank);

    size_t start;
    size_t finish;
    size_t local_fact=1;
    size_t rtn=1;

    if(Rank == 0){
        start = (size_t)2;
        finish = (size_t) (fact/worldSize);
    }else if(Rank == worldSize-1){
         
        start = (size_t) (((fact/worldSize) * Rank)+1);
        finish = (size_t) fact; 
    }else{
        start = (size_t) (((fact/worldSize) * Rank)+1);
        finish = (size_t) start + (fact/worldSize) -1;
    }

       
    size_t i, stuff;
    //puts("BEfor for loop");
    printf("Rank: %d | Start: %zu | Finish: %zu\n", Rank, start, finish);
    for(i=start; i<=finish; i++){
        stuff = i;
        
        while(stuff %10 == 0){
            stuff /= 10;
        }

        while(stuff >= 100000)
            stuff %= 100000;
         
        local_fact *= stuff;
        //printf("local_fact *= %zu: %zu\n",i,local_fact);
        while(local_fact % 10 == 0){
            local_fact /= 10;
        //    puts("oh no");
        }
        while(local_fact >= 100000){
            local_fact %= 100000;
        //    puts("local_fact >= 100000");
        }
        //if(i%200000 == 0){
        //    printf("%zu\n", i);
        //}
    }
    //puts("ENd for loop");
    

    
    
    while(local_fact % 10 == 0){
        local_fact /= 10;
        puts("oh no");
    }

    while(local_fact >= 100000){
        local_fact %= 100000;
    }  
    
   
    //MPI_PROD overflows so we need to split up the multiplication into steps. We either need to "hack" reduce 
    //function so ranks call it in certian situations (Figure out how many multiplications can be together without overflow)
    //
    //OR
    //
    //us MPI_send and MPI_rec to send local_arr to other nodes and make my own "reduce"
    //printf("local_fact: %zu\n", local_fact);
    //collector[Rank] = local_fact; 
    //puts("Before gather");
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
               
                //printf("Collector[j]: %zu\n", collector[j]); 
                while(collector[q] % 10 == 0){
                    collector[q] /= 10;
                    //puts("oh no");
                }
                if(collector[q] >= 100000){
                    collector[q] %= 100000;
                }
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

    
    size_t fac = 1000000000000;
    //size_t fac = 1000; 
    //size_t fac = 1000; 
    //puts("Start factorial!!");
    
    
    
    
    size_t result = factorial(fac, &world, worldSize, myRank);
    //size_t results = factorialTest(fac, &world, worldSize, myRank);
    if(myRank == 0){
        //printf("Ree %zu \n",results);
        printf("eeR %zu\n", result);
        printf("WORLD SIZE: %d\n", worldSize);
    }
    //M
    //puts("Done factorial!!");


    MPI_Finalize(); // Finalizing MPI

    return 0;
}
