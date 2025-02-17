#ifndef BST_H
#define BST_H

#include "doubly_linked_list.h"

// Generic BST Node
typedef struct BSTNode 
{
    void *key;   // Generic key (e.g., priority, string, etc.)
    void *data;  // Generic data (e.g., list of tasks, events, etc.)
    struct BSTNode *left, *right;
} BSTNode;

// BST Tree Structure
typedef struct BST
{
    BSTNode *root;
    int (*cmp)(void *, void *); // Function pointer for comparing keys
} BST;

// BST Functions
BST *create_tree(int (*cmp)(void *, void *));



void add_to_bst(BST *tree, void *key, void *data);  // Wrapper for inserting into BST

void *copy_key(void *key);

void *copy_data(void *data);

BSTNode *find_bst_node(BSTNode *node, void *key, int (*cmp)(void *, void *));

BSTNode *insert_into_bst(BSTNode *node, void *key, void *data, int (*cmp)(void *, void *));

BSTNode *delete_bst_node(BSTNode *node, void *key, void (*free_data)(void *), void (*free_key)(void *), 
                         int (*cmp)(void *, void *), void *(*copy_key)(void *), void *(*copy_data)(void *));

BSTNode *find_min(BSTNode *node);

void inorder_traversal(BSTNode *node);

void free_tree(BSTNode *root, void (*free_data)(void *), void (*free_key)(void *));

#endif
