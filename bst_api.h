#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <stdlib.h>
#include "doubly_linked_list.h"

#define MAX_TREE_SIZE 1000

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
    int (*cmp)(void *, void *);        // Function pointer for comparing keys
    void *(*copy_key)(void *, size_t); // Function to copy keys
    void (*free_key)(void *);          // Function to free keys
    int size;
    int capacity;
} BST;

// --- BST Utility Functions ---
BST *create_bst(int (*cmp)(void *, void *), 
    void *(*copy_key)(void *, size_t), void (*free_key)(void *), int capacity);

BSTNode *create_bst_node(BST *tree, void *key, void *data);

bool is_bst_empty(BST *tree);
int get_bst_size(BST *tree);
bool is_bst_full(BST *tree);

// Determines if new_key should go to the left of current_key
bool to_left(BST *tree, void *new_key, void *current_key);

// Determines if new_key should go to the right of current_key
bool to_right(BST *tree, void *new_key, void *current_key);

// --- BST Operations ---
BSTNode *find_min(BSTNode *node);
BSTNode *find_bst(BST *tree, BSTNode *node, void *key);
bool insert_into_bst(BSTNode **node, BST *tree, void *key, void *data);
bool delete_from_bst(BSTNode **node, BST *tree, void *key); // Internal use

// **Encapsulated Add & Remove**
BSTNode *find_min_bst(BST *tree);
BSTNode *find_bst_node(BST *tree, void *key);
bool add_to_bst(BST *tree, void *key, void *data);
bool remove_from_bst(BST *tree, void *key);

// --- BST Traversal ---
void inorder_traversal(BSTNode *node);
void traverse_tree(BST *tree);

void free_bst(BSTNode *node, void (*free_key)(void *));

#endif
