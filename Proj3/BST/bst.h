#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct MetaNode {
    char id[18];
    char title[384];
    char author[36664];
    char abstract[6093];
    int authority;//TODO make doubles
    int hub;//TODO
    int pageRank;//TODO

    struct MetaNode *right; // right child
    struct MetaNode *left; // left child
} mnode;

typedef struct WordNode {
    char word[18];//TODO make longest word length
    // Need to figure out how this will work for outgoing citations
    //char **outgoing;

    struct CiteNode *right; // right child
    struct CiteNode *left; // left child
} cnode;

// Insert for MetaNode
mnode* insertMN(mnode*, mnode*);
// Searches for a node with the corresponding ID and then returns a pointer to that node
mnode* searchMN(mnode*, char*);
// Creates a CiteNode with the given criteria
cnode* insertCN(cnode*, cnode*);
// Searches for a node with the corresponding ID and then returns a pointer to that node
cnode* searchCN(cnode*, char*);
