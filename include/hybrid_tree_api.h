#ifndef HYBRID_TREE_MAP_H
#define HYBRID_TREE_MAP_H

#include <stdbool.h>
#include "../include/hybrid_tree_api.h"
#include "../include/tree_map_api.h"

struct HashMapWithTree; // Forward declaration
struct DynamicArray;    // Forward declaration

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
    void *value;
    struct HybridNode *parent;
    struct HybridNode *child[2]; // child[LEFT] = left, child[RIGHT] = right
} HybridNode;

// Hybrid Tree structure
typedef struct HybridTree {
    struct HybridNode *root;
    int size;
    int capacity;
} HybridTree;

// At the top of hybrid_tree_api.h
typedef struct DynamicArray DynamicArray;
typedef struct TreeMap TreeMap;

// Forward declaration of HashMapWithTree
struct HashMapWithTree;


// Core Functions
struct HybridTree *create_hybrid_tree(int capacity);
struct HybridNode *create_hybrid_node(int key);
void destroy_hybrid_tree(struct HybridTree *tree);

// Insertion and Deletion
struct HybridNode* insert_hybrid(struct HybridTree* tree, struct HybridNode* node, int key, Direction dir, bool* fixup_ok);
struct HybridNode *delete_hybrid(struct HybridTree *tree, struct HybridNode *node, int key, Direction dir, bool *fixup_ok);
void insert_hybrid_public(struct HybridTree *tree, int key, bool *inserted);
void delete_from_hybrid_tree(struct HybridTree *tree, int key);
void range_query(struct HybridNode *node, int low, int high, struct DynamicArray *result);
void in_order_range_query(struct HybridNode *node, int low, int high, struct DynamicArray *result);

// Searching and Access Count Management
struct HybridNode *search_hybrid(struct HybridTree *tree, int key);
void increment_access_count(struct HybridNode *node);

struct HybridNode *rb_insert_fixup(struct HybridTree *tree, struct HybridNode *node, Direction dir);
struct HybridNode *rb_delete_fixup(struct HybridTree *tree, struct HybridNode *node, bool dir, bool *ok);
struct HybridNode *rotate(struct HybridTree *tree, struct HybridNode *node, Direction dir);
struct HybridNode *double_rotate(struct HybridTree* tree, struct HybridNode* node, Direction dir);
void color_flip(struct HybridNode *node);

// AVL Balancing Functions
int max(int a, int b);
void update_height(struct HybridNode *node);
int get_height(struct HybridNode *node);
int get_balance(struct HybridNode *node);
struct HybridNode *avl_rotate_left(struct HybridNode *node);
struct HybridNode *avl_rotate_right(struct HybridNode *node);
struct HybridNode *rebalance_if_needed(struct HybridTree *tree, struct HybridNode *node);


// Utility Functions
void free_hybrid_tree(struct HybridNode *node, void (*free_key)(void *));
void print_hybrid_tree(struct HybridNode *root, int level);
void inorder_traversal(struct HybridNode *node);

// Successor and Predecessor
struct HybridNode*find_successor(struct HybridNode *node);
struct HybridNode *find_predecessor(struct HybridNode *node);

// Minimum and Maximum Key Functions
struct HybridNode *find_minimum(struct HybridNode *node);
struct HybridNode *find_maximum(struct HybridNode *node);
void swap_keys(struct HybridNode* a, struct HybridNode* b);

// Capacity Handling
bool hybrid_tree_is_full(struct HybridTree *tree);

// --- Integration Points with TreeMap --- //

// Inserts a TreeMap key into the HybridTree for faster access
// In hybrid_tree_api.h
struct HybridNode *tree_map_insert_hybrid(struct HashMapWithTree *map, struct HybridTree *tree, int key);

// Deletes a TreeMap key from the HybridTree
void tree_map_delete_hybrid(TreeMap *map, struct HybridTree *tree, int key);

// Searches for a key in the TreeMap via the HybridTree
struct HybridNode *tree_map_search_hybrid(TreeMap *map, struct HybridTree *tree, int key);

#endif // HYBRID_TREE_MAP_H