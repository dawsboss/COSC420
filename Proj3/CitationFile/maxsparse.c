#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<mpi.h>
#include"citetree.h"

#define max_papers 1628188
#define file_length 10913892

/*
 * Script to find the MAX amount of integers that will be needed to consider
 * all rows of sparse matrix from the arxiv-citations.txt file
 */
int main() {

    FILE* fh = NULL;
    fh = fopen("arxiv-citations.txt", "r");
    FILE* fh2 = NULL;
    fh2 = fopen("arxiv-citations.txt", "r");

    if (!fh || !fh2) {
        fprintf(stderr, "One of the files broke somehow!");
        return 0;
    }


    
    char tmp[18]; // Size is the max of an id
    int numOfPapers = 0; // Must do this quick scan because not all papers
                        // from the metadata file are in the ciations file

    // Find the amount of papers in the file
    while (fgets(tmp, 18, fh2)) {
        fgets(tmp, 18, fh2);
        while(fgets(tmp, 18, fh2)) {
            if (tmp[0] == '+' && tmp[1] == '+') {
                break;
            }
        }
        numOfPapers++;
    }
    printf("The number of papers that are accounted for in the arxiv-citations.txt file are %d\n", numOfPapers);
    fclose(fh2);
    
    int max = 0;
    int countCitations = 0;
    long int paperCounter = 0;

    while (paperCounter < numOfPapers) {
        // Get rid of first two (id of first paper and -----)
        fgets(tmp, 18, fh);
        fgets(tmp, 18, fh);

        // Now check all of the citations
        while(fgets(tmp, 18, fh)) {
            if (tmp[0] == '+' && tmp[1] == '+') {
                if (countCitations > max) {
                    max = countCitations;
                }
                countCitations = 0;
                break;
            }
            countCitations++;
        }
        paperCounter++;
    }
    fclose(fh);
    printf("The max amount of citations a paper has is %d\n", max);
    printf("This means that the worst case padding is %d integers to represent each row of the sparse matrix\n", max + 1);

    return 0;
}
