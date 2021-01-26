#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<mpi.h>
#include<string.h>

typedef struct IndexNode {
    char id[18];
    long int index;
} inode;

typedef struct PageRankNode {
    char id[18];
    double rank;
} prnode;

#define num_papers 1628188

int main() {
    MPI_Init(NULL, NULL);

    FILE* readHandle = fopen("../MetadataFile/indexfile", "r");
    FILE* writeHandle = fopen("samplepagerankstructs", "w");

    if (!readHandle || !writeHandle) {
        puts("One of the files broke :(");
        MPI_Finalize();
        return 0;
    }

    // Create the array for the index of the papers to be read into
    inode* paperArray = (inode*)malloc(num_papers*sizeof(inode));
    // Read in the papers from the index file 
    fread(paperArray, sizeof(inode), num_papers, readHandle);
    fclose(readHandle);

    int counter = 0;

    // Create the array of pagerank nodes
    prnode* tmp = (prnode*)malloc(num_papers*sizeof(prnode));
    // Set the array
    memset(tmp, 0, num_papers*sizeof(prnode));
    while (counter < num_papers) {
        strcpy(tmp[counter].id, paperArray[counter].id);
        tmp[counter].rank = (double)rand() / (double)RAND_MAX;
        counter++;
    }
    // Write the array to the file
    fwrite(tmp, sizeof(prnode), num_papers, writeHandle);        
    fclose(writeHandle);
    free(paperArray);
    free(tmp);

    // Testing to see what is written to a file
    /*
    FILE* fh = fopen("samplepagerankstructs", "r");
    if (!fh)
        puts("Testing file broke");

    int i;
    prnode* samplearr = (prnode*)malloc(10*sizeof(prnode));
    fread(samplearr, sizeof(prnode), 10, fh);
    puts("Printing the contents of the samplepagerankstructs file");
    for (i = 0; i < 10; i++) {
        printf("Paper %d\n", i);
        printf("id: %s\n", samplearr[i].id);
        printf("PageRank: %f\n", samplearr[i].rank);
    }
    puts("");
    free(samplearr);
    fclose(fh);
    */

    MPI_Finalize();
    return 0;
}
