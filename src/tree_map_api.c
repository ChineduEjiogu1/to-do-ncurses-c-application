#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>  // for qsort
#include "../include/hybrid_tree_api.h"

// Basic hash function (Knuth's variant)
unsigned int hash(int key, int capacity)
{
    unsigned int knuth = (unsigned int)key * 2654435761u;
    return knuth % capacity;
}

// Create a new hash map with given capacity
HashMapWithTree *create_tree_map(int map_capacity, int tree_capacity)
{
    HashMapWithTree *map = (HashMapWithTree *)malloc(sizeof(HashMapWithTree));
    if (!map)
    {
        fprintf(stderr, "Memory allocation failed for HashMapWithTree.\n");
        return NULL;
    }

    map->capacity = map_capacity;
    map->buckets = (HybridTree **)malloc(sizeof(HybridTree *) * map_capacity);
    if (!map->buckets)
    {
        fprintf(stderr, "Memory allocation failed for buckets.\n");
        free(map);
        return NULL;
    }

    for (int i = 0; i < map_capacity; i++)
    {
        map->buckets[i] = create_hybrid_tree(tree_capacity);
        if (!map->buckets[i])
        {
            fprintf(stderr, "Failed to create HybridTree at index %d.\n", i);
            
            // Clean up previously created HybridTrees to avoid memory leaks
            for (int j = 0; j < i; j++)
            {
                free_hybrid_tree(map->buckets[j]->root, NULL);
            }

            free(map->buckets);
            free(map);
            return NULL;  // Return NULL after cleanup
        }
    }

    return map;
}

// Insert key into the tree map
bool tree_map_insert(HashMapWithTree *map, int key)
{
    if (!map)
        return false;

    unsigned int index = hash(key, map->capacity);
    bool inserted = false;
    insert_hybrid_public(map->buckets[index], key, &inserted);
    return inserted;
}

// Delete a key from the tree map
bool tree_map_delete(HashMapWithTree *map, int key)
{
    if (!map)
        return false;

    unsigned int index = hash(key, map->capacity);
    delete_from_hybrid_tree(map->buckets[index], key);
    return true;
}

// Search for a key in the tree map
bool tree_map_search(HashMapWithTree *map, int key)
{
    if (!map)
        return false;

    unsigned int index = hash(key, map->capacity);
    return search_hybrid(map->buckets[index], key) != NULL;
}

// Print the TreeMap
void tree_map_print(HashMapWithTree *map)
{
    if (!map)
        return;

    for (int i = 0; i < map->capacity; i++)
    {
        printf("Bucket %d:\n", i);
        if (map->buckets[i] && map->buckets[i]->root)
            print_hybrid_tree(map->buckets[i]->root, 0);
        else
            printf("(empty)\n");
    }
}

int compare_nodes(const void *a, const void *b) {
    const HybridNode *na = *(const HybridNode **)a;
    const HybridNode *nb = *(const HybridNode **)b;
    return na->key - nb->key;
}

// Perform range query across all HybridTrees in the map
void tree_map_range_query_ordered(HashMapWithTree *map, int low, int high, DynamicArray *result) {
    if (!map) return;

    for (int i = 0; i < map->capacity; i++) {
        range_query(map->buckets[i]->root, low, high, result);
    }

    qsort(result->items, result->size, sizeof(HybridNode *), compare_nodes);
}

// Free the map and its contents
void free_tree_map(HashMapWithTree *map)
{
    if (!map)
        return;

    // Loop through each bucket and free the HybridTree and its root
    for (int j = 0; j < map->capacity; j++) {
        free_hybrid_tree(map->buckets[j]->root, NULL);  // Pass NULL if no callback is needed
        free(map->buckets[j]);  // Free the bucket (HybridTree pointer itself)
    }

    free(map->buckets);  // Free the array of bucket pointers
    free(map);  // Finally, free the main map structure
}

int main() 
{
    HashMapWithTree *map = create_tree_map(20, 50); // 20 buckets, each HybridTree with capacity 50

    tree_map_insert(map, 10);
    tree_map_insert(map, 30);
    tree_map_insert(map, 5);

    tree_map_print(map);

    DynamicArray *result = create_dynamic_array(10);
    tree_map_range_query_ordered(map, 5, 35, result);

    printf("Result keys:\n");
    for (int i = 0; i < result->size; i++) {
        printf("%d ", result->items[i]->key);
    }
    printf("\n");

    free_dynamic_array(result);
    free_tree_map(map);
    return 0;
}