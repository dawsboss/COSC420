#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<time.h>
#include<mpi.h>
#include"citetree.h"

typedef struct IndexNode {
    char id[18];
    long int index;
} inode;

#define num_papers 1628188
#define file_length 10913892

int main() {

    MPI_Init(NULL, NULL);

    // Create the necessary file pointers
    FILE* readCitations = NULL;
    FILE* writeMat = NULL;
    //FILE* writeMatIndex = NULL; 
    FILE* readIndex = NULL;

    // Open the file to read the citation files in
    readCitations = fopen("arxiv-citations.txt", "r");
    // Read the index file to work with the matrix array
    //readIndex = fopen("indexfile", "r");
    readIndex = fopen("indexfile", "r");
    // Open the file to write the matrix into
    writeMat = fopen("matrixfile", "w");
    // Open the file to write the indices of the matrix
    //writeMatIndex = fopen("matrixfileindex", "w");

    if (!readCitations || !writeMat || !readIndex) { // If changed back, then fix this TODO
        puts("One of the three files broke :(");
        return 0;
    }

    /*
    // THIS IS FOR CREATING THE INDEX FILE BASED OFF OF THE CITATION DOCUMENT
    // Index for the indexnode struct and array counter
    long int index = 0;
    // Count the amount of papers to make sure it matches
    long int count = 0;
    // tmp array for strings
    char tmp[18];
    // Array for index nodes
    inode* writeIndex = (inode*)malloc(num_papers*sizeof(inode));
    memset(writeIndex, 0, num_papers*sizeof(inode));
    // Must create the new index first
    while (index < num_papers) {
        // TESTING
        if (!fgets(tmp, 18, readCitations)) {
            break;
        }
        inode newNode;
        // Copy the id into the index node
        strcpy(newNode.id, tmp);
        newNode.index = index;
        // Replace the \n
        newNode.id[strlen(newNode.id) - 1] = '\0';
        // Add to the array of structs
        writeIndex[index] = newNode;
        // Test print
        printf("ID: %s\n", writeIndex[index].id);
        printf("Index: %ld\n", writeIndex[index].index);
        // Skip intermediate lines (for now)
        while (fgets(tmp, 18, readCitations)) {
            if (tmp[0] == '+' && tmp[1] == '+') {
                break;       
            }
        }
        // Increment extra stuff
        index++;
        count++;
    }
    // Write the array to the specified file
    //fwrite(writeIndex, sizeof(inode), num_papers, writeMatIndex);
    printf("Count of papers read in %ld vs. known %ld\n", count, num_papers);
    free(writeIndex);
    fclose(writeMatIndex);

    // TESTING writeMatIndex file
    FILE* readTest = NULL;
    readTest = fopen("matrixfileindex", "r");
    inode* testarr = (inode*)malloc(100*sizeof(inode));
    memset(testarr, 0, 100*sizeof(inode));
    fread(testarr, sizeof(inode), 100, readTest);
    int i;
    puts("TESTING THE FILE THAT WAS WRITTEN TO...");
    for (i = 0; i < 100; i++) {
        printf("Struct %d\n" , i + 1);
        printf("id: %s\n", testarr[i].id);
        printf("index: %ld\n", testarr[i].index);
    }
    fclose(readTest);
    free(testarr);
    */

    /*
    FILE* readit = NULL;
    readit = fopen("matrixfileindex", "r");
    inode* check = (inode*)malloc(num_papers*sizeof(inode));
    memset(check, 0, num_papers*sizeof(inode));
    fread(check, sizeof(inode), num_papers, readit);
    int i;
    for (i = 0; i < num_papers; i++) {
        printf("id = %s\n", check[i].id);
    }
    free(check);
    fclose(readit);
    */


    puts("Reading arXiv citation file and creating citation tree...");
    // Create the citetree head
    cnode* head = NULL;
    // Read arxiv file into bst
    long int length = 0;
    long int byteNum;
    char insertMe[18];
    while(length < file_length) {
        byteNum = ftell(readCitations);
        fgets(insertMe, 18, readCitations);
        insertMe[strlen(insertMe) - 1] = '\0';
        insert(&head, insertMe, byteNum);
        length++;
        while(fgets(insertMe, 18, readCitations)) {
            length++;
            if (insertMe[0] == '+' && insertMe[1] == '+') {
                break;
            }
        }
    }
    puts("Finished reading arXiv citation file and creating citation tree...");
    /*
     * TESTS FOR TREE BEFORE ALGORITHM
    // Print the tree
    print(head);
    char test[] = {'p','l','a','s','m','-','p','h','/','9','5','1','2','0','0','2','\0'};
    printf("Searching for %s\n", test);
    long int rtn = search(&head, test);
    printf("Got this: %ld\n", rtn);
    puts("Testing seek based off the bytecode returned");
    fseek(readCitations, rtn, SEEK_SET);
    fgets(insertMe, 18, readCitations);
    printf("String should match above %s\n", insertMe);
    clearTree(&head);

    fclose(readCitations);
    fclose(writeMat);
    fclose(readIndex);
    return 0;
    */

    // Timing
    time_t first, second;
    puts("Reading in struct matrix and creating binary tree...");
    // Create the pointer to hold each row of the matrix
    char* row = NULL;
    // Create the index array to keep track of each index and read it in
    // Then add it to the tree that holds the metadata
    inode* indices = (inode*)malloc(num_papers*sizeof(inode));
    memset(indices, 0, num_papers*sizeof(inode));
    fread(indices, sizeof(inode), num_papers, readIndex);
    fclose(readIndex);
    long int p;
    snode* head2 = NULL;
    long int semibenchmark = 200000;
    for (p = 0; p < num_papers; p++) {
        //printf("Hi %ld\n", p);
        inserts(&head2, indices[p].id, indices[p].index);
        if (p % semibenchmark == 0) {
            printf("benchmark: %ld\n", p);
        }
    }
    /*
    snode* head2 = NULL;
    char needMe[18];
    char takeMe[36664];
    long int coo = 0;
    while(coo < num_papers) {
        if (coo % 10000 == 0) {
            printf("%ld\n", coo);
        }
        fgets(needMe, 18, readIndex);
        needMe[strlen(needMe) - 1] = '\0';
        inserts(&head2, needMe, coo);
        fgets(takeMe, 36664, readIndex);
        fgets(takeMe, 36664, readIndex);
        fgets(takeMe, 36664, readIndex);
        fgets(takeMe, 36664, readIndex);
        coo++;
    }
    // Testing the second tree
    prints(head2);
    clearTree(&head);
    clearTrees(&head2);
    fclose(readCitations);
    fclose(writeMat);
    free(indices);
    return 0;
    */
    puts("Finished reading in struct matrix and creating binary tree...");
    puts("Running matrix file creation algorithm...");
    // Done reading the index structs in
    // Count the amount of papers to make sure it matches
    long int count = 0;
    // tmp array for strings
    char tmp[18];
    // Counter
    long int i;
    // Byte search variable
    long int byteSearch = 0;
    // Index for tree search
    long int indexForOne = 0;
    // Test bool
    //bool allZeros = false;
    //bool shouldPrint = false;
    // Benchmark number
    long int benchmark = 100000;
    // Start the loop
    first = time(NULL);
    while (count < num_papers) {
        // We have a tree of where to find every paper in the file
        // SO LETS USE IT BABY
        byteSearch = search(&head, indices[count].id);
        row = (char*)malloc(num_papers*sizeof(char));
        memset(row, 0, sizeof(char));
        // Initialize the row
        for (i = 0; i < num_papers; i++) {
            row[i] = '0';
        }
        if (byteSearch == -1) {
            fwrite(row, sizeof(char), num_papers, writeMat);
            fseek(readCitations, 0, SEEK_SET);
            //shouldPrint = false;
            count++;
            free(row);
            continue;
        }
        fseek(readCitations, byteSearch, SEEK_SET);
        /*
        // Need to iterate over the amount of papers in the indexfile
        // and find the right paper id that matches in the arxiv file
        while (fgets(tmp, 18, readCitations)) {
            // Kill the endline so somparisons work
            tmp[strlen(tmp) - 1] = '\0';
            if (strcmp(indices[count].id, tmp) == 0) {
                puts("Found it!!!!!");
                printf("struct id: %s\nfound id: %s\n", indices[count].id, tmp);
                break;
            }
            // Skip all of the other elements until we get to the "next" paper for 
            // citation checking
            while(fgets(tmp, 18, readCitations)) {
                if (tmp[0] == '+' && tmp[1] == '+') {
                    break;
                }
            }
            // If we reach the end of the file, the loops will hit NULL and skip!!!!
        }
        */
        // We know the structure and the order already
        // Will be the paper we know and a minus row next... skip it!!!!
        fgets(tmp, 18, readCitations);
        fgets(tmp, 18, readCitations); 
        // If we couldnt find the string, just put an all zero row in its place
        // Read all of the citations in and fill the row vector until we read the
        // dividing line
        while(fgets(tmp, 18, readCitations)) {
            if (tmp[0] == '+' && tmp[1] == '+') {
                break;
            }
            tmp[strlen(tmp) - 1] = '\0';
            /*
            for (i = 0; i < num_papers; i++) {
                if (strcmp(tmp, indices[i].id) == 0) {
                    row[i] = 1; 
                }
            }
            */
            indexForOne = searchs(&head2, tmp);
            // Not enough time to run on full case, if not enough then continue
            if (indexForOne == -1) {
                //puts("failed");
                continue;
            }
            row[indexForOne] = '1';
            // Print it out
            //shouldPrint = true; 
        }
        // Write the row to the file after the matrix row has been filled
        fwrite(row, sizeof(char), num_papers, writeMat);
        /*
        // Test Print
        if (shouldPrint) {
            // Test
            printf("Paper %ld\n", count);
            for (i = 0; i < num_papers; i++) {
                if (row[i] == 1) {
                    puts("we have a winner!");
                }
            }
        }
        */
        // Reset the pointer to the beginning of the file
        fseek(readCitations, 0, SEEK_SET);
        //shouldPrint = false;
        count++;
        free(row);
        if (count % benchmark == 0) {
            printf("Benchmark %ld\n", count);
        }
    }
    second = time(NULL);
    printf("Count of papers read in %ld vs. known %ld\n", count, num_papers);
    printf("Time in seconds to complete: %ld\n", second - first);
    puts("Clearing all of the memory...");
    free(indices);
    clearTree(&head);
    clearTrees(&head2);

    // Close the files
    fclose(readCitations);
    fclose(writeMat);

    /*
    puts("This is a just a test to show that the file is being written to correctly, every 'we have a winner' is a 1 found in the row of paper #, only 100 being printed so it doesnt take forever");
    // TEST matrix write file
    puts("STARTING TEST OF WRITE FILE");
    FILE* yeet = NULL;
    yeet = fopen("matrixfile", "r");
    double* readme = NULL;
    long int j;
    int sumcheck = 0;
    for (i = 0; i < 100; i++) {
        readme = (double*)malloc(num_papers*sizeof(double));
        memset(readme, 0, num_papers*sizeof(double));
        fread(readme, sizeof(double), num_papers, yeet);
        for (j = 0; j < num_papers; j++) {
            sumcheck += readme[j];
            if (readme[j] == 1) {
                printf("Paper %ld\n", i);
                puts("we have a winner!");
            }
        }
        if (sumcheck == 0) {
            printf("Paper %ld\n", i);
            puts("all zeros row");
        }
        sumcheck = 0;
        free(readme);
    }
    fclose(yeet);
    */

    MPI_Finalize();
    
    return 0;
}
