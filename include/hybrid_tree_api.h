#ifndef HYBRID_TREE_MAP_H
#define HYBRID_TREE_MAP_H

#include <stdbool.h>
#include "tree_map_api.h"  // To enable interaction with TreeMap

// Define Red-Black tree colors
#define RED 0
#define BLACK 1

// Hybrid Node structure combining AVL and Red-Black properties
typedef struct HybridNode {
    int key;
    int color; // 0 = Red, 1 = Black
    int height; // For AVL balancing
    int access_count; // Tracks frequent access for AVL balancing

    struct HybridNode *left;
    struct HybridNode *right;
    struct HybridNode *parent;
} HybridNode;

// Hybrid Tree structure
typedef struct HybridTree {
    HybridNode *root;
    int size;
    int capacity;
} HybridTree;

// Core Functions
HybridTree *create_hybrid_tree(int capacity);
HybridNode *create_hybrid_node(int key);

// Insertion and Deletion
void insert_hybrid(HybridTree *tree, int key);
void delete_hybrid(HybridTree *tree, int key);

// Searching and Access Count Management
HybridNode *search_hybrid(HybridTree *tree, int key);
void increment_access_count(HybridNode *node);

// Red-Black Balancing Functions
void rb_insert_fixup(HybridTree *tree, HybridNode *node);
void rb_delete_fixup(HybridTree *tree, HybridNode *node);
void rb_rotate_left(HybridTree *tree, HybridNode *x);
void rb_rotate_right(HybridTree *tree, HybridNode *y);

// AVL Balancing Functions
int max(int a, int b);
void update_height(HybridNode *node);
int get_height(HybridNode *node);
int get_balance(HybridNode *node);
HybridNode *avl_rotate_left(HybridNode *node);
HybridNode *avl_rotate_right(HybridNode *node);
void rebalance_if_needed(HybridTree *tree, HybridNode *node);

// Utility Functions
void free_hybrid_tree(HybridNode *root);
void print_hybrid_tree(HybridNode *root, int level);

// Successor and Predecessor
HybridNode *find_successor(HybridNode *node);
HybridNode *find_predecessor(HybridNode *node);

// Minimum and Maximum Key Functions
HybridNode *find_minimum(HybridNode *node);
HybridNode *find_maximum(HybridNode *node);

// Capacity Handling
bool hybrid_tree_is_full(HybridTree *tree);

// --- Integration Points with TreeMap --- //

// Inserts a TreeMap key into the HybridTree for faster access
void tree_map_insert_hybrid(TreeMap *map, HybridTree *tree, int key);

// Deletes a TreeMap key from the HybridTree
void tree_map_delete_hybrid(TreeMap *map, HybridTree *tree, int key);

// Searches for a key in the TreeMap via the HybridTree
HybridNode *tree_map_search_hybrid(TreeMap *map, HybridTree *tree, int key);

#endif // HYBRID_TREE_MAP_H
