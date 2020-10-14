#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<math.h>
#include<limits.h> //MAX_INT
//16576

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME

typedef unsigned long long boi;
typedef int bool;
#define true 1
#define false 0

static boi lim = 4294967295;

boi overflow(boi a, boi b, int mod, bool first){
    boi rtn;
    //puts("overflew");    printf("    Wokring on a: %llu | b: %llu\n", a , b);
    printf("    Wokring on a: %llu | b: %llu\n", a , b);
    if(a==0 || b==0)
      return 0;
    if (a>lim) {//if a will make me oveflow recurse
        puts("a>lim");
        boi k = a/lim;
        boi rem = a%lim;
        printf("k %llu, rem %llu, a %llu, b %llu\n", k, rem, a, b);
        rtn = (overflow(overflow(k,b, mod, false), lim, mod, false) + overflow(rem, b, mod, false));
    }else if(b>lim){// if b is going ot make me overflow recurse on it
        puts("b>lim");
        boi k = b/lim;
        boi rem = b%lim;
        printf("k %llu, rem %llu, a %llu, b %llu\n", k, rem, a, b);    
        rtn = (overflow(overflow(k,a, mod, false), lim, mod, false) + overflow(rem, a, mod, false));
    }else{//Multiplication is safe at this point
        rtn = (a*b);
    }
    printf("rtn Before: %llu, a: %llu, b: %llu\n", rtn,a,b);
    
    if(first){
        while(rtn%10 == 0)
            rtn/=10;
        printf("rtn After: %llu\n", rtn);
    }
    rtn%=mod;

    printf("    Returning rtn: %llu\n", rtn);
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
        local_fact = overflow(local_fact, i, 100000, true);
    }
    printf("local_fact: %llu\n", local_fact);
    MPI_Gather(&local_fact, 1, MPI_UNSIGNED_LONG_LONG, collector, 1, MPI_UNSIGNED_LONG_LONG, 0, *world);
    

    if(Rank == 0){
        int w, q;
        for(w=2; w<(2*worldSize); w*=2){
            for(q=0; q<worldSize - w/2; q+=w){
               
                collector[q] = overflow(collector[q], collector[q+w/2], 100000, true);
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

    puts("Testing");

    boi result;
    boi fac; 
    fac = 50000; 
    
    boi one = 5000000000;//ULLONG_MAX;
    boi two = one-1;//ULLONG_MAX;
    
    boi jawn = overflow(one, two, 100000, true);
   
    printf("Jawn: %llu\n", jawn); 
    
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
