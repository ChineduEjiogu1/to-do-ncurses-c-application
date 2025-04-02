#ifndef HYBRID_TREE_MAP_H
#define HYBRID_TREE_MAP_H

#include <stdbool.h>
#include "hybrid_tree_api.h"  // To enable interaction with TreeMap
#include "../include/tree_map_api.h"

// Define Red-Black tree colors using enum for better readability
typedef enum { RED, BLACK } Color;

// Define direction constants for clarity
typedef enum { LEFT = 0, RIGHT = 1 } Direction;

// Hybrid Node structure combining AVL and Red-Black properties
typedef struct HybridNode {
    int key;
    Color color;       // Used for Red-Black balancing
    int height;        // Used for AVL balancing
    int access_count;  // Tracks frequent access for AVL optimizations

    struct HybridNode *parent;
    struct HybridNode *child[2]; // child[LEFT] = left, child[RIGHT] = right
} HybridNode;

// Hybrid Tree structure
typedef struct hybrid_tree {
    HybridTree *root;
    int size;
    int capacity;
} HybridTree;

// Core Functions
HybridTree *create_hybrid_tree(int capacity);
HybridNode *create_hybrid_node(int key);
void destroy_hybrid_tree(HybridTree *tree);

// Insertion and Deletion
void insert_hybrid(HybridTree *tree, int key);
void delete_hybrid(HybridTree *tree, int key, Direction dir);

// Searching and Access Count Management
HybridNode *search_hybrid(HybridTree *tree, int key);
void increment_access_count(HybridNode *node);

HybridNode *rb_insert_fixup(HybridTree *tree, HybridNode *node, Direction dir);
HybridNode *rb_delete_fixup(HybridTree *tree, HybridNode *node, Direction dir);
HybridNode *rotate(HybridTree *tree, HybridNode *node, Direction dir);
HybridNode *double_rotate(HybridTree*tree, HybridNode *node, Direction dir);
void color_flip(HybridNode *node);

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
HybridNode*find_successor(HybridNode *node);
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
