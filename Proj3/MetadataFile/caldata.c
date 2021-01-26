#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define FILE_LINES 8140940

int main() {
    
    // Find the largest char counts in the file
    FILE* fh = NULL;

    fh = fopen("arxiv-metadata.txt", "r");

    if (fh == NULL) {
        fprintf(stderr, "Error opening file");
        return 0;
    }
    
    char* findLength = (char*)malloc(150000*sizeof(char));
    int count = 1;
    int maxStrLen[] = {0,0,0,0,0};
    int tmp;
    double fileLineLength = 0;
    long int is = 0;
    int i;
    while (is < FILE_LINES) {
        fgets(findLength, 150000, fh);
        if (count == 1) {
            tmp = strlen(findLength); 
            if (tmp > maxStrLen[0]) {
                maxStrLen[0] = tmp;
            }
        } else if (count == 2) {
            tmp = strlen(findLength); 
            if (tmp > maxStrLen[1]) {
                maxStrLen[1] = tmp;
            }
        } else if (count == 3) {
            tmp = strlen(findLength); 
            if (tmp > maxStrLen[2]) {
                maxStrLen[2] = tmp;
            }
        } else if (count == 4) {
            tmp = strlen(findLength); 
            if (tmp > maxStrLen[3]) {
                maxStrLen[3] = tmp;
            }
        } else {
            tmp = strlen(findLength); 
            if (tmp > maxStrLen[4]) {
                maxStrLen[4] = tmp;
            }
            count = 0; 
        }
        free(findLength);
        findLength = (char*)malloc(150000*sizeof(char));
        count++;
        fileLineLength++;
        is++;
    }
    puts("All 5 longest fields");
    for (i = 0; i < 5; i++) {
        printf("%d ", maxStrLen[i]);
    }
    puts("");
    puts("# of lines in the file:");
    printf("%f", fileLineLength);
    puts("");
    fclose(fh);

    return 0;
}
