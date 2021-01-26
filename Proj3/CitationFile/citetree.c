#include"citetree.h"

//----------------------------------CITENODE--------------------------------
// Inserts a node into the citetree with a given string and line index
void insert(cnode** head, char* insertString, long int insertIndex) {
    cnode* newNode = (cnode*)malloc(sizeof(cnode));
    strcpy(newNode->id, insertString);
    newNode->line = insertIndex;
    newNode->right = NULL;
    newNode->left = NULL;

    cnode* cursor = *head;
    cnode* trail = cursor;

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
long int search(cnode** head, char* searchString) {
    cnode* cursor = *head;

    if (cursor == NULL) {
        return -1;
    }
    while(cursor) {
        if (strcmp(searchString, cursor->id) == 0) {
            return cursor->line;
        } else if (strcmp(searchString, cursor->id) < 0) {
            cursor = cursor->left;
        } else {
            cursor = cursor->right;
        }
    }

    // If we make it to here, than there is nothing in the arxiv file, return -1
    return -1;
}

// Deletes the dynamic memory contained by the tree
void clearTree(cnode** head) {
    cnode* cursor = *head; 
    cnode* up = NULL;

    while(cursor) {
        if (cursor->left != NULL) {
            cnode* left = cursor->left;
            cursor->left = up;
            up = cursor;
            cursor = left;
        } else if (cursor->right != NULL) {
            cnode* right = cursor->right;
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
void print(cnode* head) {
    if (head != NULL) {
        printf("Id: %s\n", head->id);
        printf("Line number: %ld\n", head->line) ;
        print(head->left);
        print(head->right);
    }
}

//----------------------------------STORENODE--------------------------------
// Inserts a node into the citetree with a given string and line index
void inserts(snode** head, char* insertString, long int insertIndex) {
    snode* newNode = (snode*)malloc(sizeof(snode));
    strcpy(newNode->id, insertString);
    newNode->index = insertIndex;
    newNode->right = NULL;
    newNode->left = NULL;

    snode* cursor = *head;
    snode* trail = cursor;

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
long int searchs(snode** head, char* searchString) {
    snode* cursor = *head;

    if (cursor == NULL) {
        return -1;
    }
    while(cursor) {
        if (strcmp(searchString, cursor->id) == 0) {
            return cursor->index;
        } else if (strcmp(searchString, cursor->id) < 0) {
            cursor = cursor->left;
        } else {
            cursor = cursor->right;
        }
    }

    // This should not happen :/
    return -1;
}

// Deletes the dynamic memory contained by the tree
void clearTrees(snode** head) {
    snode* cursor = *head; 
    snode* up = NULL;

    while(cursor) {
        if (cursor->left != NULL) {
            snode* left = cursor->left;
            cursor->left = up;
            up = cursor;
            cursor = left;
        } else if (cursor->right != NULL) {
            snode* right = cursor->right;
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
void prints(snode* head) {
    if (head != NULL) {
        printf("Id: %s\n", head->id);
        printf("Index: %ld\n", head->index) ;
        prints(head->left);
        prints(head->right);
    }
}
