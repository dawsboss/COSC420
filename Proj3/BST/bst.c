#include"bst.h"

// Inserts given MetaNode into the binary tree
// root is a pointer to a MetaNode
// insertMe is a node that is passed by reference to the function
mnode* insertMN(mnode* root, mnode* insertMe) {
    if (root == NULL) {
        root = insertMe;
        return root;
    } else if (strcmp(insertMe->id, root->id) > 0) {
        root->right = insertMN(root->right, insertMe); 
    } else {
        root->left = insertMN(root->left, insertMe);
    }
    return root;
}

// Searches for a node that matches the corresponding Id
// root is a pointer to a MetaNode
// searchId is a cstring array acting as the search term
mnode* searchMN(mnode* root, char* searchId) {
    if (root == NULL || strcmp(root->id, searchId) == 0) {
        return root;
    } else if (strcmp(searchId, root->id)) {
        return searchMN(root->right, searchId);
    } else {
        return searchMN(root->left, searchId);
    }
}

// Inserts given CiteNode into the binary tree
// root is a pointer to a CiteNode
// insertMe is a node that is passed by reference to the function
cnode* insertCN(cnode* root, cnode* insertMe) {
    return NULL;
}

// Searches for a node that matches the corresponding Id
// root is a pointer to a CiteNode
// searchId is a cstring array acting as the search term
cnode* searchCN(cnode* root, char* searchId) {
    return NULL;
}
