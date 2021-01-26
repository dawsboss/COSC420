#include"citetree.h"

//----------------------------------MDTREENODE--------------------------------
// Inserts a node into the citetree with a given string and line index
void insert(mdtreenode** head, char* insertId, char* insertTitle, char* insertAuthor, char* insertAbstract, double insertPR) {
    mdtreenode* newNode = (mdtreenode*)malloc(sizeof(mdtreenode));
    strcpy(newNode->id, insertId);
    strcpy(newNode->title, insertTitle);
    strcpy(newNode->author, insertAuthor);
    strcpy(newNode->abstract, insertAbstract);
    newNode->pageRank = insertPR;
    newNode->right = NULL;
    newNode->left = NULL;

    mdtreenode* cursor = *head;
    mdtreenode* trail = cursor;

    if (cursor == NULL) {
        *head = newNode;
        return;
    }
    
    while(cursor) {
        trail = cursor;
        if (strcmp(newNode->id, cursor->id) <= 0) {
            cursor = cursor->left; 
        } else {
            cursor = cursor->right;
        }
    }
    
    if (strcmp(newNode->id, trail->id) <= 0) {
        trail->left = newNode;    
    } else {
        trail->right = newNode;
    }
}

// Searches for a given string in the tree and returns the line index of that node
mdnode* search(mdtreenode** head, char* searchId) {
    mdtreenode* cursor = *head;

    if (cursor == NULL) {
        return NULL;
    }
    while(cursor) {
        if (strcmp(searchId, cursor->id) == 0) {
            mdnode* rtnMe = (mdnode*)malloc(sizeof(mdnode));;
            strcpy(rtnMe->id, cursor->id);
            strcpy(rtnMe->title, cursor->title);
            strcpy(rtnMe->author, cursor->author);
            strcpy(rtnMe->abstract, cursor->abstract);
            rtnMe->pageRank = cursor->pageRank;
            return rtnMe;
        } else if (strcmp(searchId, cursor->id) < 0) {
            cursor = cursor->left;
        } else {
            cursor = cursor->right;
        }
    }

    // If we make it to here, than there is nothing in the arxiv file, return NULL
    return NULL;
}

// Deletes the dynamic memory contained by the tree
void clearTree(mdtreenode** head) {
    mdtreenode* cursor = *head; 
    mdtreenode* up = NULL;

    while(cursor) {
        if (cursor->left != NULL) {
            mdtreenode* left = cursor->left;
            cursor->left = up;
            up = cursor;
            cursor = left;
        } else if (cursor->right != NULL) {
            mdtreenode* right = cursor->right;
            cursor->left = up;
            cursor->right = NULL;
            up = cursor;
            cursor = right;
        } else {
            if (up == NULL) {
                free(cursor);
                cursor = NULL;
            }
            while (up != NULL) {
                free(cursor);
                if (up->right != NULL) {
                    cursor = up->right;
                    up->right = NULL;
                    break;
                } else {
                    cursor = up;
                    up = up->left;
                }
            }
        }
    }
    *head = NULL;
}

// Prints the tree recursively (ONLY USE ON SMALL CASES)
void print(mdtreenode* head) {
    if (head != NULL) {
        printf("Id: %s\n", head->id);
        printf("Title: %s\n", head->title);
        printf("Authors: %s\n", head->author);
        printf("Abstract: %s\n", head->abstract);
        printf("PageRank: %f\n", head->pageRank);
        print(head->left);
        print(head->right);
    }
}

//----------------------------------WNODE--------------------------------
// Inserts a node into the citetree with a given string and line index
void inserts(wnode** head, char* insertWord) {
    wnode* newNode = (wnode*)malloc(sizeof(wnode));
    strcpy(newNode->word, insertWord);
    // Will have some sort of insert/initialization for the sub-data structure
    newNode->right = NULL;
    newNode->left = NULL;

    wnode* cursor = *head;
    wnode* trail = cursor;

    if (cursor == NULL) {
        *head = newNode;
        return;
    }
    
    while(cursor) {
        trail = cursor;
        if (strcmp(newNode->word, cursor->word) <= 0) {
            cursor = cursor->left; 
        } else {
            cursor = cursor->right;
        }
    }
    
    if (strcmp(newNode->word, trail->word) <= 0) {
        trail->left = newNode;    
    } else {
        trail->right = newNode;
    }
}

// Searches for a given string in the tree and returns the line index of that node
void* searchs(wnode** head, char* searchWord) { 
    wnode* cursor = *head;

    if (cursor == NULL) {
        return NULL; // TODO 
    }
    while(cursor) {
        if (strcmp(searchWord, cursor->word) == 0) {
            return NULL; // TODO Will need to return the data structure here or some sort of array that is filled
        } else if (strcmp(searchWord, cursor->word) < 0) {
            cursor = cursor->left;
        } else {
            cursor = cursor->right;
        }
    }

    // This should not happen :/
    return NULL; // TODO
}

// Deletes the dynamic memory contained by the tree
void clearTrees(wnode** head) {
    wnode* cursor = *head; 
    wnode* up = NULL;

    while(cursor) {
        if (cursor->left != NULL) {
            wnode* left = cursor->left;
            cursor->left = up;
            up = cursor;
            cursor = left;
        } else if (cursor->right != NULL) {
            wnode* right = cursor->right;
            cursor->left = up;
            cursor->right = NULL;
            up = cursor;
            cursor = right;
        } else {
            if (up == NULL) {
                free(cursor);
                cursor = NULL;
            }
            while (up != NULL) {
                free(cursor);
                if (up->right != NULL) {
                    cursor = up->right;
                    up->right = NULL;
                    break;
                } else {
                    cursor = up;
                    up = up->left;
                }
            }
        }
    }
    *head = NULL;
}

// Prints the tree recursively (ONLY USE ON SMALL CASES)
void prints(wnode* head) {
    if (head != NULL) {
        printf("Word: %s\n", head->word);
        // TODO Will print the data structure here
        prints(head->left);
        prints(head->right);
    }
}
