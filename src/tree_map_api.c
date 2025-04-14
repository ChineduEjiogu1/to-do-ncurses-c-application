#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../include/tree_map_api.h"
#include "../include/dynamic_array_api.h"

// Simple hash function for integer keys (updated to take capacity)
unsigned int hash(int key, int capacity) {
    return key % capacity;
}

// Create a new HashMap with HybridTrees as buckets
HashMapWithTree *create_tree_map(int capacity) {
    HashMapWithTree *map = (HashMapWithTree *)malloc(sizeof(HashMapWithTree));
    if (!map)
        return NULL;

    map->capacity = capacity;
    map->buckets = (struct HybridTree **)malloc(sizeof(struct HybridTree *) * capacity);
    if (!map->buckets) {
        free(map);
        return NULL;
    }

    for (int i = 0; i < capacity; i++) {
        map->buckets[i] = create_hybrid_tree(20);  // You can change this tree capacity
    }

    return map;
}

// Insert a key-value pair into the HashMap
bool tree_map_insert(HashMapWithTree *map, int key) {
    if (!map)
        return false;

    unsigned int index = hash(key, map->capacity);
    bool inserted = false;
    insert_hybrid_public(map->buckets[index], key, &inserted);
    return inserted;
}

// Delete a key from the HashMap
bool tree_map_delete(HashMapWithTree *map, int key) {
    if (!map)
        return false;

    unsigned int index = hash(key, map->capacity);
    delete_from_hybrid_tree(map->buckets[index], key);
    return true;
}

// Search for a key in the HashMap
bool tree_map_search(HashMapWithTree *map, int key) {
    if (!map)
        return false;

    unsigned int index = hash(key, map->capacity);
    return (search_hybrid(map->buckets[index], key) != NULL);
}

// Print the entire HashMap (all buckets)
void tree_map_print(HashMapWithTree *map) {
    if (!map) return;

    for (int i = 0; i < map->capacity; i++) {
        printf("Bucket %d:\n", i);
        print_hybrid_tree(map->buckets[i]->root, 0);
    }
}

// Perform a range query on the entire HashMap
void tree_map_range_query(struct HashMapWithTree *map, int low, int high, DynamicArray *result) {
    if (!map) return;

    printf("Keys in range [%d, %d]: ", low, high);
    
    for (int i = 0; i < map->capacity; i++) {
        range_query(map->buckets[i]->root, low, high, result);
    }

    printf("\n");
}

// Free the HashMap and all its resources
void free_tree_map(HashMapWithTree *map) {
    if (!map)
        return;

    for (int i = 0; i < map->capacity; i++) {
        free_hybrid_tree(map->buckets[i]->root, NULL);
        free(map->buckets[i]);
    }

    free(map->buckets);
    free(map);
}

// int main() {
//     // Create a HashMap with HybridTree buckets
//     HashMapWithTree *map = create_tree_map(20);

//     // Insert some keys
//     tree_map_insert(map, 5);
//     tree_map_insert(map, 3);
//     tree_map_insert(map, 9);
//     tree_map_insert(map, 8);
//     tree_map_insert(map, 12);
//     tree_map_insert(map, 18);
//     tree_map_insert(map, 5);
//     tree_map_insert(map, 11);

//     // Print the entire map
//     tree_map_print(map);

//     // Perform a range query
//     DynamicArray *range = create_dynamic_array(10);
//     tree_map_range_query(map, 4, 10, range);

//     // Print the range query results
//     printf("Range query result:\n");
//     for (int i = 0; i < range->size; i++) {
//         printf("%d ", range->items[i]->key);
//     }
//     printf("\n");

//     // Delete a key
//     tree_map_delete(map, 3);
//     printf("After deletion:\n");
//     tree_map_print(map);

//     // Free resources
//     free_dynamic_array(range);
//     free_tree_map(map);

//     return 0;
// }