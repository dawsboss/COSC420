#ifndef _CITETREE_H_
#define _CITETREE_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// For holding the nodes while being passed around in main
typedef struct MetaDataNode {
    char id[18];
    char title[384];
    char author[36664];
    char abstract[6093];
    double pageRank;
} mdnode;

// Node that contains all of the MetaDataNode info but has
// attributes that can be entered into the tree
typedef struct MetaDataTreeNode {
    char id[18];
    char title[384];
    char author[36664];
    char abstract[6093];
    double pageRank;

    struct MetaDataTreeNode *right;
    struct MetaDataTreeNode *left;
} mdtreenode;

// Node that will be indexed by words and will have a data structure (to be determined) that will contain 
// paper id's that contain that word
typedef struct WordNode {
    char word[25]; // TODO Might need to create a script to find the largest word amongst all of the abstracts
    // Some type of data structure to hold all of the id's of the abstracts that contain the current word    

    struct WordNode *right;
    struct WordNode *left;
} wnode;

//--------------MDNODE-------------
// Inserts a node into the tree 
void insert(mdtreenode**, char*, char*, char*, char*, double);
// Searches for a node in the tree by id, and returns all of the
// data (MetaDataNode) regarding that paper that has that id
mdnode* search(mdtreenode**, char*);
// Clears the tree
void clearTree(mdtreenode**);
// Print (ONLY USE FOR SMALL CASES)
void print(mdtreenode*);

//--------------WNODE-------------
// Inserts a node into the tree
void inserts(wnode**, char*);
// Searches for a node in the tree by word that is tokenized by an abstract and returns a data structure
// that is yet to be determined containing the id's of paper's that contain that word
void* searchs(wnode**, char*);
// Clears the tree
void clearTrees(wnode**);
// Print (ONLY USE FOR SMALL CASES)
void prints(wnode*);

#endif
