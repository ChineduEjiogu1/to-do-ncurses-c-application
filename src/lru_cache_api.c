#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "../include/lru_cache_api.h"
#include "../include/tree_map_api.h"
#include "../include/doubly_linked_list.h"

// Define the LRU Cache structure

// Create a new LRU Cache
LRUCache *lru_cache_create(size_t capacity)
{
    LRUCache *cache = (LRUCache *)malloc(sizeof(LRUCache));
    cache->map = create_tree_map(100, 100); // or whatever tree capacity you want
    cache->dll = create_list(100);
    cache->capacity = capacity;
    return cache;
}

Data *create_data(void *value)
{
    Data *data = (Data *)malloc(sizeof(Data));
    if (!data)
        return NULL;
    data->value = value;
    return data;
}

// Get a value from the cache
int lru_cache_get(LRUCache *cache, int key)
{
    // Convert the integer key to void* to match the tree map's key type
    LRUNode *node = (LRUNode *)tree_map_search(cache->map, key);

    if (node == NULL)
    {
        return -1; // Key not found
    }

    // Move the node to the front of the list by removing it and re-inserting at the front
    delete_by_value(cache->dll, node); // Remove the node from the list
    insert_front(cache->dll, create_data(node)); // Re-insert the node at the front

    return void_ptr_to_int(node->value); // Return the value associated with the key
}

// Put a key-value pair into the cache
void lru_cache_put(LRUCache *cache, int key, int value)
{
    // Search for the key in the map (tree_map)
    LRUNode *node = (LRUNode *)tree_map_search(cache->map, key);

    if (node != NULL)
    {
        // If the key is found, update the value and move it to the front of the list
        node->value = int_to_void_ptr(value);
        insert_front(cache->dll, create_data(node));
    }
    else
    {
        // If the key is not found and the cache is at capacity, evict the LRU item
        if (list_size(cache->dll) >= cache->capacity)
        {
            // Find the LRU (least recently used) node, which is at the tail of the list
            Node *tail_node = cache->dll->tail;
            LRUNode *lru = (LRUNode *)tail_node->data->value;

            // Remove the LRU item from the tree map and doubly linked list
            tree_map_delete(cache->map, void_ptr_to_int(lru->key));
            delete_by_value(cache->dll, lru);  // Ensure this properly updates the DLL
            free(lru);  // Free the LRU node memory
        }

        // Create a new node for the key-value pair
        LRUNode *new_node = (LRUNode *)malloc(sizeof(LRUNode));
        new_node->key = int_to_void_ptr(key);
        new_node->value = int_to_void_ptr(value);

        // Insert the new node at the front of the doubly linked list
        insert_front(cache->dll, create_data(new_node));

        // Insert the new node into the tree map
        tree_map_insert(cache->map, key, new_node);
    }
}


// Destroy the LRU Cache
void lru_cache_destroy(LRUCache *cache)
{
    free_list(cache->dll);
    free_tree_map(cache->map);
    free(cache);
}

int main()
{
    // Create a new LRU Cache with a capacity of 3
    LRUCache *cache = lru_cache_create(3);
    
    // Insert some values into the cache
    printf("Putting key=1, value=10\n");
    lru_cache_put(cache, 1, 10);
    
    printf("Putting key=2, value=20\n");
    lru_cache_put(cache, 2, 20);
    
    printf("Putting key=3, value=30\n");
    lru_cache_put(cache, 3, 30);
    
    // Get values from the cache
    printf("Getting key=1: %d\n", lru_cache_get(cache, 1));  // Should return 10
    printf("Getting key=2: %d\n", lru_cache_get(cache, 2));  // Should return 20

    // Add another key, should evict the least recently used (key=3)
    printf("Putting key=4, value=40\n");
    lru_cache_put(cache, 4, 40);
    
    // Attempt to get the evicted key
    printf("Getting key=3: %d\n", lru_cache_get(cache, 3));  // Should return -1 (not found)

    // Adding a key that should evict key 1
    printf("Putting key=5, value=50\n");
    lru_cache_put(cache, 5, 50);
    
    // Test again
    printf("Getting key=1: %d\n", lru_cache_get(cache, 1));  // Should return -1 (not found)
    printf("Getting key=4: %d\n", lru_cache_get(cache, 4));  // Should return 40
    printf("Getting key=5: %d\n", lru_cache_get(cache, 5));  // Should return 50

    // Clean up the cache
    lru_cache_destroy(cache);
    
    return 0;
}
