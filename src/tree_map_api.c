#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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
            for (int j = 0; j < i; j++)
            {
                free_hybrid_tree(map->buckets[j]->root, NULL);
                free(map->buckets[j]);
            }
            free(map->buckets);
            free(map);
            return NULL;
        }
    }

    return map;
}

// Insert key into the tree map
bool tree_map_insert(HashMapWithTree *map, int key, void *value)
{
    if (!map)
        return false;

    unsigned int index = hash(key, map->capacity);
    printf("Inserting key %d into bucket %u...\n", key, index);

    bool inserted = false;
    insert_hybrid_public(map->buckets[index], key, value, &inserted);

    if (inserted)
    {
        printf("Key %d inserted successfully into bucket %u.\n", key, index);
    }
    else
    {
        printf("Failed to insert key %d into bucket %u.\n", key, index);
    }

    return inserted;
}

// Delete a key from the tree map
bool tree_map_delete(HashMapWithTree *map, int key)
{
    if (!map)
    {
        fprintf(stderr, "Error: HashMapWithTree is NULL.\n");
        return false;
    }

    unsigned int index = hash(key, map->capacity);
    printf("Deleting key %d from bucket %u...\n", key, index);

    HybridTree *tree = map->buckets[index];
    if (!tree || !tree->root)
    {
        printf("Tree at bucket %u is empty, nothing to delete.\n", index);
        return false;
    }

    delete_from_hybrid_tree(tree, key);
    if (!tree_map_search(map, key))
    {
        printf("Key %d deleted successfully from bucket %u.\n", key, index);
        return true;
    }
    else
    {
        printf("Failed to delete key %d from bucket %u.\n", key, index);
        return false;
    }
}

// Search for a key in the tree map
struct HybridNode *tree_map_search(HashMapWithTree *map, int key)
{
    if (!map)
    {
        fprintf(stderr, "Error: HashMapWithTree is NULL.\n");
        return false; // Return false if map is NULL
    }

    unsigned int index = hash(key, map->capacity);
    printf("Searching for key %d in bucket %u...\n", key, index);

    HybridNode *node = search_hybrid(map->buckets[index], int_to_void_ptr(key));
    if (node)
    {
        printf("Key %d found in bucket %u.\n", key, index);
        return node; // Return true if the key is found
    }
    else
    {
        printf("Key %d not found in bucket %u.\n", key, index);
        return node; // Return false if the key is not found
    }
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

int compare_nodes(const void *a, const void *b)
{
    const HybridNode *na = *(const HybridNode **)a;
    const HybridNode *nb = *(const HybridNode **)b;
    return void_ptr_to_int(na->key) - void_ptr_to_int(nb->key);
}

// Perform range query across all HybridTrees in the map
void tree_map_range_query_ordered(HashMapWithTree *map, int low, int high, DynamicArray *result)
{
    if (!map)
        return;

    for (int i = 0; i < map->capacity; i++)
    {
        range_query(map->buckets[i]->root, low, high, result);
    }

    qsort(result->items, result->size, sizeof(HybridNode *), compare_nodes);
}

// Free the map and its contents
void free_tree_map(HashMapWithTree *map)
{
    if (!map)
        return;

    for (int j = 0; j < map->capacity; j++)
    {
        free_hybrid_tree(map->buckets[j]->root, NULL);
        free(map->buckets[j]);
    }

    free(map->buckets);
    free(map);
}

void print_range_query_result(DynamicArray *result)
{
    if (!result || result->size == 0)
    {
        printf("Range query result is empty.\n");
        return;
    }

    printf("Range query result:\n");
    for (int i = 0; i < result->size; ++i)
    {
        HybridNode *node = (HybridNode *)result->items[i];
        if (!node)
        {
            printf("Warning: NULL node at index %d\n", i);
            continue;
        }

        int key = void_ptr_to_int(node->key);
        if (!node->value)
            printf("Key: %d, Value: NULL\n", key);
        else
            printf("Key: %d, Value: %s\n", key, (char *)node->value);
    }
}

void perform_range_query_and_print(HybridTree *tree, int low, int high)
{
    DynamicArray *result = create_dynamic_array(10);
    printf("Range query [%d, %d]:\n", low, high);
    range_query(tree->root, low, high, result);
    print_range_query_result(result);
    free_dynamic_array(result);
}

// Main function to test tree_map_api
// int main()
// {
//     int map_capacity = 5;
//     int tree_capacity = 10;
//     HashMapWithTree *tree_map = create_tree_map(map_capacity, tree_capacity);

//     if (!tree_map)
//     {
//         fprintf(stderr, "Failed to create tree map.\n");
//         return EXIT_FAILURE;
//     }

//     // Insert key-value pairs
//     printf("Inserting key-value pairs into the tree map...\n");
//     tree_map_insert(tree_map, 1, strdup("Value1"));
//     tree_map_insert(tree_map, 2, strdup("Value2"));
//     tree_map_insert(tree_map, 3, strdup("Value3"));
//     tree_map_insert(tree_map, 4, strdup("Value4"));
//     tree_map_insert(tree_map, 5, strdup("Value5"));

//     // Print the tree map
//     printf("\nTree map after insertion:\n");
//     tree_map_print(tree_map);

//     // Search for a key
//     printf("\nSearching for key 3 in the tree map...\n");
//     HybridNode *node = tree_map_search(tree_map, 3);
//     if (node && node->value)
//     {
//         printf("Found key 3 with value: %s\n", (char *)node->value);
//     }
//     else
//     {
//         printf("Key 3 not found.\n");
//     }

//     // Delete a key
//     printf("\nDeleting key 2 from the tree map...\n");
//     if (tree_map_delete(tree_map, 2))
//     {
//         printf("Key 2 deleted successfully.\n");
//     }
//     else
//     {
//         printf("Failed to delete key 2.\n");
//     }

//     // Print the tree map again
//     printf("\nTree map after deletion:\n");
//     tree_map_print(tree_map);

//     // Perform a range query
//     printf("\nPerforming a range query from 1 to 4:\n");
//     DynamicArray *range_result = create_dynamic_array(10);
//     tree_map_range_query_ordered(tree_map, 1, 4, range_result);
//     print_range_query_result(range_result);

//     // Free the tree map
//     free_tree_map(tree_map);

//     return EXIT_SUCCESS;
// }