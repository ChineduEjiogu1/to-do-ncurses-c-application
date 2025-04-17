// tree_map.h

#ifndef TREE_MAP_H
#define TREE_MAP_H

#include "../include/dynamic_array_api.h"
#include "../include/hybrid_tree_api.h"

typedef struct DynamicArray DynamicArray;

// Define the number of buckets in the hash map
#define BUCKET_SIZE 1000

// Define the HashMap structure with multiple Hybrid Trees (Red-Black Trees)
typedef struct HashMapWithTree {
    struct HybridTree **buckets;  // Change from static array to pointer for flexibility
    int capacity;                 // Store the actual capacity (number of buckets)
} HashMapWithTree;


// Function declarations
HashMapWithTree *create_tree_map(int map_capacity, int tree_capacity);
unsigned int hash(int key, int capacity);  // Create a new tree map
bool tree_map_insert(HashMapWithTree *map, int key); // Insert key into the tree map
bool tree_map_delete(HashMapWithTree *map, int key);  // Delete key from the tree map
void free_tree_map(HashMapWithTree *map);  // Free all resources of the tree map
bool tree_map_search(HashMapWithTree *map, int key);  // Search for a key in the tree map
void tree_map_print(HashMapWithTree *map);  // Print the tree map (all buckets)
void tree_map_range_query_ordered(HashMapWithTree *map, int low, int high, DynamicArray *result);  // Range query for the tree map
 // Range query for the tree map

#endif // TREE_MAP_H