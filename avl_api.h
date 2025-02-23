#ifndef AVL_H
#define AVL_H

#include <stdbool.h>
#include <stdlib.h>
#include "include/doubly_linked_list.h"

#define MAX_TREE_SIZE 1000

// --- Structure to Hold Multiple Values Per Key ---
typedef struct AVLValueList 
{
    void **values;  // Dynamic array of values
    size_t size;    // Number of values
    size_t capacity;// Capacity of the array
} AVLValueList;

// --- Generic AVL Tree Node ---
typedef struct AVLNode 
{
    void *key;           // Generic key (e.g., priority, date, etc.)
    AVLValueList *list;  // List of values instead of a single data pointer
    struct AVLNode *left, *right;
    struct AVLNode *parent;
    int height;
} AVLNode;

// --- AVL Tree Structure ---
typedef struct AVL 
{
    AVLNode *root;
    int (*cmp)(void *, void *);        // Function pointer for comparing keys
    void *(*copy_key)(void *, size_t); // Function to copy keys
    void (*free_key)(void *);          // Function to free keys
    int size;
    int capacity;
} AVL;

// --- AVLValueList Functions ---
AVLValueList *create_value_list(int capacity);
void free_value_list(AVLValueList *list);
bool add_value_to_list(AVLValueList *list, void *data); // Added function
bool remove_value_from_list(AVLValueList *list, void *data); // Added function

// --- AVL Tree Utility Functions ---
AVL *create_avl(int (*cmp)(void *, void *), 
    void *(*copy_key)(void *, size_t), void (*free_key)(void *), int capacity);
AVLNode *create_avl_node(AVL *tree, void *key, void *data);
bool is_avl_empty(AVL *tree);
int get_avl_size(AVL *tree);
bool is_avl_full(AVL *tree);
int max(int a, int b);

// Determines if new_key should go to the left of current_key
bool to_left(AVL *tree, void *new_key, void *current_key);

// Determines if new_key should go to the right of current_key
bool to_right(AVL *tree, void *new_key, void *current_key);

// --- AVL Tree Operations ---
AVLNode *find_min(AVLNode *node); // Added function
AVLNode *find_avl(AVL *tree, AVLNode *node, void *key); // Added function
bool insert_into_avl(AVLNode **node, AVL *tree, void *key, void *data);
bool delete_from_avl(AVLNode **node, AVL *tree, void *key, void *value); // Updated

// **Encapsulated Add & Remove**
AVLNode *find_min(AVLNode *node);
AVLNode *find_avl(AVL *tree, AVLNode *node, void *key);
bool add_to_avl(AVL *tree, void *key, void *data);
bool remove_from_avl(AVL *tree, void *key, void *value); // Updated

// --- AVL Tree Balancing Functions ---
int get_height(AVLNode *node);
int get_balance_factor(AVLNode *node);
AVLNode *rotate_right(AVLNode *y);
AVLNode *rotate_left(AVLNode *x);
AVLNode *balance_avl(AVLNode *node);

// --- AVL Tree Traversal ---
void inorder_traversal(AVLNode *node);
void traverse_tree(AVL *tree);
void free_avl(AVLNode *node, void (*free_key)(void *));

#endif
