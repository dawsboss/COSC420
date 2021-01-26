#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<string.h>
#include"citetree.h"

// THIS PROGRAM WAS CREATED TO RUN SPECIFICALLY ON 10 NODES

#define num_papers 1628188

int main() {

    MPI_Init(NULL, NULL);

    MPI_Comm world = MPI_COMM_WORLD;
    
    int myRank, worldSize;

    MPI_Comm_size(world, &worldSize);
    MPI_Comm_rank(world, &myRank);

    char myFileName[] = "../MetadataFile/metastruct ";
    myFileName[strlen(myFileName) - 1] = '0' + myRank;

    printf("myRank is %d and my file name is %s\n", myRank, myFileName);

    FILE* fh = fopen(myFileName, "r");
    
    // Make sure file worked
    if (!fh) {
        printf("myRank %d file did not open or something broke :(\n", myRank);
        MPI_Finalize();
        return 0;
    }

    int myMdnodeSize = 10; //num_papers / 10; TODO CHANGE BACK FOR FULL VERSION
    /* TODO
    if (myRank == 9) {
        myMdnodeSize += num_papers % 10;
    }
    */

    // Create the array for each node
    mdnode* myStructArray = (mdnode*)malloc(myMdnodeSize*sizeof(mdnode));
    memset(myStructArray, 0, myMdnodeSize*sizeof(mdnode));
    printf("myRank %d made it after the initialization of the array and my array size is %d\n", myRank, myMdnodeSize);

    // Make sure malloc didnt die
    if (!myStructArray) {
        printf("myRank %d array did not initialize for whatever reason :(\n", myRank);
        MPI_Finalize();
        return 0;
    }

    //printf("myRank %d made it before the reading in of the file to the array\n!", myRank);
    fread(myStructArray, sizeof(mdnode), myMdnodeSize, fh);
    //printf("myRank %d made it after the reading in of the file to the array!\n", myRank);
    int i;
    /*
    for (i = 0; i < myMdnodeSize; i++) { // TODO This will replace the 'reading' in of the pagerank file for now
        myStructArray[i].pageRank = (double)rand() / (double)RAND_MAX;
    }
    mdtreenode* paperTree = NULL; // Create the tree
    for (i = 0; i < myMdnodeSize; i++) {
        printf("%s\n", myStructArray[i].id);
        insert(&paperTree, myStructArray[i].id, myStructArray[i].title, myStructArray[i].author, myStructArray[i].abstract, myStructArray[i].pageRank);
    }
    //print(paperTree);
    */

    // TEST to make sure that the files are getting things correctly
    /*
    for (i = 0; i < myMdnodeSize; i++) {
        printf("myRank %d, struct num %d, id is %s\n", myRank, i, myStructArray[i].id); 
    }
    */
    // Insert into the binary search tree containing all of the paper metadata

    
    // Each node now will parse their abstracts for the second tree.
    // Strtok will affect the original string, everything should be inserted into the paper tree. Therefore,
    // we dont need to worry about what happens to the array of all of the papers
    char delim[] = " ";
    char *word = NULL;
    for (i = 0; i < myMdnodeSize; i++) {
        // Start the string token up
        word = strtok(myStructArray[i].abstract, delim);
        while (word != NULL) {
            // ADD TESTS AND ADD TO THE TREE HERE
            printf("myRank: %d Abstract %d: %s\n", myRank, i, word);
            word = strtok(NULL, delim);
        }
    }

    // Free the array of meta node structs
    free(myStructArray);
    // Free the file pointer holding the metastruct files
    fclose(fh);

    MPI_Finalize();
    return 0;
}
