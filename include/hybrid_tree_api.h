#ifndef HYBRID_TREE_MAP_H
#define HYBRID_TREE_MAP_H

#include <stdbool.h>
#include "../include/hybrid_tree_api.h"
#include "../include/tree_map_api.h"

typedef struct HashMapWithTree HashMapWithTree; // Forward declaration
struct DynamicArray;    // Forward declaration
// Forward declaration of HashMapWithTree

// Define Red-Black tree colors using enum for better readability
typedef enum
{
    RED,
    BLACK
} Color;

// Define direction constants for clarity
typedef enum
{
    LEFT = 0,
    RIGHT = 1
} Direction;

// Hybrid Node structure combining AVL and Red-Black properties
typedef struct HybridNode
{
    void *key;
    Color color;      // Used for Red-Black balancing
    int height;       // Used for AVL balancing
    int access_count; // Tracks frequent access for AVL optimizations
    void *value;
    struct HybridNode *parent;
    struct HybridNode *child[2]; // child[LEFT] = left, child[RIGHT] = right
} HybridNode;

// Hybrid Tree structure
typedef struct HybridTree
{
    struct HybridNode *root;
    int size;
    int capacity;
} HybridTree;

// At the top of hybrid_tree_api.h
typedef struct DynamicArray DynamicArray;

// Core Functions
HybridTree *create_hybrid_tree();
void destroy_hybrid_tree(struct HybridTree *tree);
int void_ptr_to_int(void *ptr);
void *int_to_void_ptr(int key);

// Insertion and Deletion
HybridNode *insert_hybrid(HybridTree *tree, HybridNode *node, int key, void *value, Direction dir, bool *fixup_ok);
HybridNode *delete_hybrid(HybridTree *tree, HybridNode *node, int key, Direction dir, bool *fixup_ok);
void insert_hybrid_public(HybridTree *tree, int key, void *value, bool *inserted);
void delete_from_hybrid_tree(HybridTree *tree, int key);
void range_query(HybridNode *node, int low, int high, DynamicArray *result);

// Searching and Access Count Management
// hybrid_tree_api.h
HybridNode *search_hybrid(HybridTree *tree, void *key);
void increment_access_count(HybridNode *node);

HybridNode *rb_insert_fixup(HybridTree *tree, HybridNode *node, Direction dir);
HybridNode *rb_delete_fixup(HybridTree *tree, HybridNode *node, bool dir, bool *ok);
HybridNode *rotate(HybridTree *tree, HybridNode *node, Direction dir);
HybridNode *double_rotate(HybridTree *tree, HybridNode *node, Direction dir);
void color_flip(HybridNode *node);

// AVL Balancing Functions
int max(int a, int b);
void update_height(HybridNode *node);
int get_height(HybridNode *node);
int get_balance(HybridNode *node);
HybridNode *avl_rotate_left(HybridNode *node);
HybridNode *avl_rotate_right(HybridNode *node);
HybridNode *rebalance_if_needed(HybridTree *tree, HybridNode *node);

// Utility Functions
void free_hybrid_tree(HybridNode *node, void (*free_key)(void *));
void print_hybrid_tree(HybridNode *root, int level);
void inorder_traversal(HybridNode *node);

// Successor and Predecessor
HybridNode *find_successor(HybridNode *node);
HybridNode *find_predecessor(HybridNode *node);

// Minimum and Maximum Key Functions
HybridNode *find_minimum(HybridNode *node);
HybridNode *find_maximum(HybridNode *node);
void swap_keys(HybridNode *a, HybridNode *b);

// Capacity Handling
bool hybrid_tree_is_full(HybridTree *tree);

// --- Integration Points with TreeMap --- //

// Inserts a TreeMap key into the HybridTree for faster access
// In hybrid_tree_api.h
HybridNode *tree_map_insert_hybrid(HashMapWithTree *map, HybridTree *tree, int key, void *value);

// Deletes a TreeMap key from the HybridTree
void tree_map_delete_hybrid(HashMapWithTree *map, HybridTree *tree, int key);

// Searches for a key in the TreeMap via the HybridTree
HybridNode *tree_map_search_hybrid(HashMapWithTree *map, HybridTree *tree, int key);

#endif // HYBRID_TREE_MAP_H