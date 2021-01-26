#ifndef _CITETREE_H_
#define _CITETREE_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct CiteNode {
    char id[18]; // sort the bst by id
    long int line; // Line in arXiv-citations.txt file that it is located at

    struct CiteNode *right; // right child
    struct CiteNode *left; // left child
} cnode;

typedef struct StoreNode {
    char id[18]; // sort the bst by id from id in indices array
    long int index; // index of the node

    struct StoreNode *right; // right child
    struct StoreNode *left; // left child
} snode;

//--------------CITENODE-------------
// Inserts a node into the tree 
void insert(cnode**, char*, long int);
// Searches for a node in the tree by id, and returns the position 
// it is found in the arxiv file
long int search(cnode**, char*);
// Clears the tree
void clearTree(cnode**);
// Print (ONLY USE FOR SMALL CASES)
void print(cnode*);

//--------------STORENODE-------------
// Inserts a node into the tree
void inserts(snode**, char*, long int);
// Searches for anode in the tre by id, and returns the index that 
// corresponds to that id in an "array"
long int searchs(snode**, char*);
// Clears the tree
void clearTrees(snode**);
// Print (ONLY USE FOR SMALL CASES
void prints(snode*);

#endif
