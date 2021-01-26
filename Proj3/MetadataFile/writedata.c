#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>

typedef struct MetaDataNode {
    char id[18];
    char title[384];
    char author[36664];
    char abstract[6093];
} mdnode;

typedef struct IndexNode {
    char id[18];
    long int index;
} inode;

#define FILE_LINES 8140940

int main() {

    // Metadata has been calculated, now need to create the structs, fix the strings, 
    // and then write them to a file to be used later
    FILE* readHandle = NULL;
    FILE* writeHandle = NULL;
    FILE* writeSubscript = NULL;

    // Time the process
    time_t first, second;
    first = time(NULL);

    readHandle = fopen("arxiv-metadata.txt", "r");
    //writeHandle = fopen("metastructs", "w");
    writeSubscript = fopen("indexfile", "w");

    if (readHandle == NULL) {
        fprintf(stderr, "Error opening file");
        return 0;
    }

    long int myIndex = 0; // Index for each paper
    long int is = 0;
    int i, j, l;
    long int count = 0; // Count the iterations for testing
    long int cap = FILE_LINES / 5;
    printf("# of papers: %ld\n", cap);
    long int divider = 10; // Dividing into how many arrays
    long int chunkSize = cap / divider; // Size of array on each iteration
    printf("# of iterations to complete (# per chunk): %ld\n", chunkSize);
    // Iterate over each chunk
    mdnode *arr = NULL;
    inode *yont = NULL;
    char tmp[10];
    for (l = 0; l < divider; l++) {
        // Create the unique file name 
        char tmpName[12];
        memset(tmpName, 0, 12*sizeof(char));
        strcat(tmpName, "metastruct");
        tmpName[10] = '0' + l;
        tmpName[11] = '\0';
        printf("Writing to %s\n", tmpName);
        // Open the unique file
        writeHandle = fopen(tmpName, "w");
        is = 0; // Need to remember to reset the inner while counter
        // Need to pick up the stragglers on chunksize (8 missing papers)
        if (l == (divider - 1)) {
            chunkSize += cap % divider;
        }
        arr = (mdnode*)malloc(chunkSize*sizeof(mdnode));
        yont = (inode*)malloc(chunkSize*sizeof(inode));
        // Make sure malloc does not break
        if (!arr) {
            puts("Malloc returned NULL");
        }
        memset(arr, 0, chunkSize*sizeof(mdnode));
        memset(yont, 0, chunkSize*sizeof(inode));
        while (is < chunkSize) {
            // Create a temp node
            mdnode newNode;
            // Create a index node
            inode indNode;
            // Get all of the strings for that node
            fgets(newNode.id, 18, readHandle);
            fgets(newNode.title, 384, readHandle);
            fgets(newNode.author, 36664, readHandle);
            fgets(newNode.abstract, 6093, readHandle);
            // Get rid of the +'s
            fgets(tmp, 10, readHandle);
            // Edit each field to get rid of it's \n char
            // Cheat it! strlen returns the length of the string up to the null terminator
            // The \n is at the end of every line, therefore strlen(node) - 1 index should be
            // where the \n is on every data member
            newNode.id[strlen(newNode.id) - 1] = '\0';
            newNode.title[strlen(newNode.title) - 1] = '\0';
            newNode.author[strlen(newNode.author) - 1] = '\0';
            // Edit the abstract to to what we want
            // i.e. get rid of all non aplphanumberic characters
            // and change the \n into a \0
            for (j = 0; j < strlen(newNode.abstract); j++) {
                if (newNode.abstract[j] == '\n') {
                    newNode.abstract[j] = '\0';
                    continue;
                }
                if (!isalpha(newNode.abstract[j]) && !isdigit(newNode.abstract[j])) {
                    newNode.abstract[j] = ' ';
                }
            }
            // Now assign all of the data to the array element 'is' of array arr
            strcpy(arr[is].id, newNode.id);
            strcpy(arr[is].title, newNode.title);
            strcpy(arr[is].author, newNode.author);
            strcpy(arr[is].abstract, newNode.abstract);
            // Now assign to index node as well
            strcpy(yont[is].id, newNode.id);
            yont[is].index = myIndex;
            // Increment for the loop
            is++;
            count++;
            myIndex++;
        }
        /*
        // Test print
        for (i = 0; i < divider; i++) {
            printf("ID: %s\n", arr[i].id);
            printf("Title: %s\n", arr[i].title);
            printf("Author(s): %s\n", arr[i].author);
            printf("Abstract: %s\n", arr[i].abstract);
            puts("======================");
            printf("ID INODE: %s\n", yont[i].id);
            printf("INDEX: %ld\n", yont[i].index);
        }
        */
        // Now write this "chunk of array" to the file
        fwrite(arr, sizeof(mdnode), chunkSize, writeHandle);
        fwrite(yont, sizeof(inode), chunkSize, writeSubscript);
        free(arr);
        free(yont);
        fclose(writeHandle);
        printf("Count at this point: %ld\n", count);
    }
    second = time(NULL);

    fclose(readHandle);
    //fclose(writeHandle);
    fclose(writeSubscript);
    printf("Count of iterations: %ld\n", count);
    printf("Total number of nodes: %ld\n", cap);
    printf("Time in seconds to complete: %ld\n", second - first);

    

    /*
    // Test to make sure the structs were written to the file correctly
    FILE* fh = NULL;
    FILE* fh2 = NULL;
    //fh2 = fopen("indexfile", "r");
    //inode* yeet2 = (inode*)malloc(100*sizeof(inode));
    //memset(yeet2, 0, 100*sizeof(inode));
    //fread(yeet2, sizeof(inode), 100, fh2);
    for (j = 0; j < divider; j++) {
        char tmpName[12];
        memset(tmpName, 0, 12*sizeof(char));
        strcat(tmpName, "metastruct");
        tmpName[10] = '0' + j;
        tmpName[11] = '\0';
        printf("Opening %s\n", tmpName);
        fh = fopen(tmpName, "r");
        mdnode* yeet = (mdnode*)malloc(10*sizeof(mdnode));
        memset(yeet, 0, 10*sizeof(mdnode));
        fread(yeet, sizeof(mdnode), 10, fh);
        puts("PRINTING STRUCTS FROM THE FILE");
        for (i = 0; i < 10; i++) {
            printf("ID: %s\n", yeet[i].id);
            printf("Title: %s\n", yeet[i].title);
            printf("Author(s): %s\n", yeet[i].author);
            printf("Abstract: %s\n", yeet[i].abstract);
            //printf("ID FROM INDEX FILE: %s\n", yeet2[i].id);
            //printf("index: %ld\n", yeet2[i].index);
        }
        free(yeet);
        //free(yeet2);
        fclose(fh);
        //fclose(fh2);
    }
    */
    
    return 0;
}
