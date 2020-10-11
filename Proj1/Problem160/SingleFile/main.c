#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<math.h>
#include<limits.h> //MAX_INT
#include "BigIntegerSingleFile.cpp"
#include<string.h>
#include<time.h>
//16576

// Defining the buffer length for processor name
#define pNameSize MPI_MAX_PROCESSOR_NAME

typedef BigInteger* boi;



//watch -n1 squeue
//sbatch P160.sh

boi factorial(unsigned long long fact, MPI_Comm *world, int worldSize, int Rank){
    char* collector = NULL;

    //printf("worldSize:%d Rank:%d\n", worldSize, Rank);

    boi zero = makeBigIntStr("0");
    boi ten = makeBigIntStr("10");
    boi one = makeBigIntStr("1");
    boi tenK = makeBigIntStr("1000000000000000"); 

    unsigned long long srt;
    unsigned long long fin;
    boi start;
    boi finish;
    boi local_fact = makeBigIntStr("1");
    boi rtn;
    char str[256];

    if(Rank == 0){
        start = makeBigIntStr("2");
        
        fin = (unsigned long long) (fact/worldSize);
        sprintf(str, "%llu", fin);   
        finish = makeBigIntStr(str);
    }else if(Rank == worldSize-1){
        srt = (unsigned long long) (((fact/worldSize) * Rank)+1);
        sprintf(str, "%llu", srt);   
        start = makeBigIntStr(str);

        fin = fact;
        sprintf(str, "%llu", fin);
        finish = makeBigIntStr(str); 
    }else{
        srt = (unsigned long long) (((fact/worldSize) * Rank)+1);
        sprintf(str, "%llu", srt);   
        start = makeBigIntStr(str);
       
         
        fin = (unsigned long long) (srt + (fact/worldSize)-1);
        //printf("fin %llu | str: %llu", fin, srt);
        sprintf(str, "%llu", fin);   
        finish = makeBigIntStr(str);
    }

    char* printfinish = c_str(finish);
    char* printstart = c_str(start);
    printf("\tline 65 Rank: %d | start: %s | finish: %s\n",Rank, printstart, printfinish);   
    del_str(printstart);
    del_str(printfinish);

    boi i;
    //printf("Rank: %d | Start: %llu | Finish: %llu\n", Rank, start, finish);
    for(i=start; 1 == c_leeq(i,finish); c_pp(i)){
        c_mult(local_fact, i);
        boi modu = c__mod(local_fact, ten);        
        while((c_eqeq(modu, zero)) == 1){
            //printf("LOOP == local_fact: %s | i: %s\n", c_str(local_fact), c_str(i));
            del(modu);
            c_div(local_fact, ten);
            modu = c__mod(local_fact, ten);
        }
        del(modu);
        if(c_greq(local_fact, tenK) == 1)
            c_mod(local_fact, tenK);
        //printf("local_fact: %s | i: %s | Rank %d\n", c_str(local_fact), c_str(i), Rank);
    }
    //printf("71 %d\n", Rank);
    printf("local_fact: \n");
    int numLength[worldSize];
    //printf("local_fact: %s\n", c_str(local_fact));
    char* reee = c_str(local_fact);
    unsigned long long ln = strlen(reee);
    del_str(reee);
    //puts("76");
    MPI_Allgather(&ln, 1, MPI_INT, numLength, 1, MPI_INT, *world);

    printf("79 %d\n", Rank); 
    //Acumulator loop for disp
    int disp[worldSize];
    int s;
    int sum=0;
    for(s=0; s<worldSize; s++){
        disp[s] = sum;
        sum += numLength[s];
    }
    const char* local_num = c_str(local_fact);
    if(Rank == 0)
      collector = (char*) malloc(sum*sizeof(char));//TODO This is broken


    printf("local_num = %s | Rank: %d \n", local_num, Rank);

    MPI_Gatherv(local_num, numLength[Rank], MPI_CHAR, collector, numLength, disp, MPI_CHAR, 0, *world);//TODO
    //printf("local_fact del %d",Rank);
    del(local_fact);
    //del_str(local_num);
    if(Rank == 0){ 
        boi collectorV2[worldSize];
        //I have an array of sum of numLens digits, I need ot slice out worlSize number fo substrings and convert the back to bois
        //strncpy() coppy n bytes from a buffer from on to another buffer
        // 
        
        int ree;
        for(ree=0; ree<worldSize; ree++){
            char num [numLength[ree]+1];
            strncpy(num, collector+disp[ree], numLength[ree]);
            num[numLength[ree]] = '\0';
            //printf("word: %s\n", num);
            collectorV2[ree] = makeBigIntStr(num);
        }
       
    //puts("collect");
    
        int w, q;
        for(w=2; w<(2*worldSize); w*=2){
            for(q=0; q<worldSize - w/2; q+=w){
                c_mult(collectorV2[q], collectorV2[q+w/2]);
                boi moduu = c__mod(collectorV2[q], ten);
                while((c_eqeq(moduu, zero)) == 1){
                    c_div(collectorV2[q], ten);
                    del(moduu);
                    moduu = c__mod(collectorV2[q], ten);
                }
                del(moduu);
                c_mod(collectorV2[q], tenK);
                
            }
        }
        //puts("After collect");
        rtn = collectorV2[0]; 
        //puts("rtn = ");
        //printf("%s\n", c_str(rtn));
        //c_mod(rtn, tenK);
        for(w=1; w<worldSize; w++){
            del(collectorV2[w]);
        } 
        del(one);
        del(ten);
        del(zero);
        del(tenK);
        del(start);
        del(finish);
        if(Rank == 0)
          free(collector);
        //puts("mod");
        return rtn;
    }
    return NULL;
}














int main(int argc, char** argv) {

    MPI_Init(&argc, &argv); // Initializing MPI

    MPI_Comm world = MPI_COMM_WORLD; // Communicator

    int worldSize, myRank;

    MPI_Comm_size(world, &worldSize); // Gives amount of nodes running in parallel

    
    MPI_Comm_rank(world, &myRank); // Gives the rank (number) node
    srand(time(0));                           // Gives the processor name of current node

    //p
    //puts("Testing");
    clock_t start;
    if(myRank == 0){
       start = clock();
    }

    boi result;
    boi finalMod = makeBigIntStr("100000");
    unsigned long long fac; 
    
    
    //fac = 100000000;
    fac = 1000000000000; 
    printf("fac: %llu",fac);
    //fac = fac / pow(5.0,7.0);

    result = factorial(fac, &world, worldSize, myRank);
    if(myRank == 0){
        clock_t end = clock();
        double timeSpent = (double) (end-start) / CLOCKS_PER_SEC;
        const char* reesult = c_str(result);
        printf("Fac: %llu | Result: %s\n", fac, reesult);
        //c_mod(result, finalMod);
        //printf("Ree %zu \n",results);
        //printf("Fac: %llu | Result: %s\n", fac, reesult);
        printf("WORLD SIZE: %d | Time: %f \n", worldSize, timeSpent);
        //del_str(reesult);   
    }
    //M
    //puts("Done factorial!!");
    /*if(myRank == 0){
        if(result == 16576){
            puts("NUT");
        }else{
            puts("REE");
        }
    }*/
    if(myRank == 0){
        del(result);
        del(finalMod);
    }
    MPI_Finalize(); // Finalizing MPI

    return 0;
}
