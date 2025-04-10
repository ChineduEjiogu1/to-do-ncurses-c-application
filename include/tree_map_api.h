#ifndef TREE_MAP_H
#define TREE_MAP_H

#include <stdbool.h>
#include "doubly_linked_list.h"
#include "hybrid_tree_api.h" // For interaction points

// struct HybridTree HybridTree;  // Forward declaration

#define MAX_TABLE_SIZE 1000

// Status codes for function results
typedef enum {
    INSERT_SUCCESS,
    INSERT_FAILURE,
    INSERT_DUPLICATE,
    DELETE_SUCCESS,
    DELETE_FAILURE,
    SEARCH_SUCCESS,
    SEARCH_NOT_FOUND
} TreeMapStatus;

// AVL Tree Node structure for efficient bucket handling
typedef struct AVLNode {
    int key;
    Node *node; // Pointer to the doubly linked list node
    struct AVLNode *left;
    struct AVLNode *right;
    int height;
} AVLNode;

// TreeMap structure combining AVL and HybridTree for optimized storage
typedef struct TreeMap {
    int capacity;
    int size;
    float load_factor;
    AVLNode **buckets; // Buckets now point to AVL Trees
    struct HybridTree *hybrid_tree; // Link to HybridTree for optimized operations
} TreeMap;

// TreeMap functions
TreeMap *create_tree_map(int capacity);

AVLNode *create_avl_node(int key, Node *node);

int hash_function(int capacity, int key);

bool needs_resizing(TreeMap *map);
TreeMapStatus resize_tree_map(TreeMap *map);

TreeMapStatus insert_into_tree_map(TreeMap *map, int key, Node *node);
AVLNode *insert_into_avl(AVLNode *root, int key, Node *node);

TreeMapStatus delete_from_tree_map(TreeMap *map, int key);
AVLNode *delete_from_avl(AVLNode *root, int key);

AVLNode *find_avl_node(AVLNode *root, int key);
TreeMapStatus search_in_tree_map(TreeMap *map, int key, Node **result);

int tree_map_size(TreeMap *map);
bool tree_map_is_empty(TreeMap *map);

void print_tree_map_table(TreeMap *map);
void free_avl_tree(AVLNode *root);
void free_tree_map(TreeMap*map);

// Interaction Points with HybridTree
TreeMapStatus insert_into_hybrid_from_treemap(TreeMap *map, int key, Node *node);
TreeMapStatus delete_from_hybrid_via_treemap(TreeMap *map, int key);
TreeMapStatus search_in_hybrid_from_treemap(TreeMap *map, int key, Node **result);

#endif // TREE_MAP_H
