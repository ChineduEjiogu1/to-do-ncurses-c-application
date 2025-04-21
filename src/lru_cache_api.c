#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

#include "../include/lru_cache_api.h"
#include "../include/tree_map_api.h"
#include "../include/doubly_linked_list.h"

// Create a new LRUNode
LRUNode *create_lru_node(void *key, void *value)
{
    LRUNode *node = malloc(sizeof(LRUNode));
    node->key = key;
    node->value = value;
    node->list_node = NULL;
    return node;
}

// Create a Data struct to wrap LRUNode*
Data *create_data(LRUNode *node)
{
    Data *data = malloc(sizeof(Data));
    data->value = node;
    return data;
}

// Create LRU Cache
LRUCache *create_lru_cache(int capacity)
{
    LRUCache *cache = malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->dll = create_list(100);
    cache->map = create_tree_map(10, 30);
    return cache;
}

// Get from LRU Cache
void *lru_cache_get(LRUCache *cache, int key)
{
    HybridNode *node_ptr = tree_map_search(cache->map, key);
    if (!node_ptr)
    {
        printf("Key %d not found in cache\n", key);
        return NULL;
    }

    LRUNode *node = (LRUNode *)node_ptr->value;

    // Move this node to the front of the DLL
    if (node->list_node)
        delete_by_value(cache->dll, node->list_node->data);

    node->list_node = insert_front(cache->dll, create_data(node));

    return node->value;
}

// Put into LRU Cache
void lru_cache_put(LRUCache *cache, int key, int value)
{
    //printf("Putting key=%d, value=%d\n", key, value);
    HybridNode *node_ptr = tree_map_search(cache->map, key);
    LRUNode *node = node_ptr ? (LRUNode *)node_ptr->value : NULL;

    if (node)
    {
        // Update value
        node->value = int_to_void_ptr(value);

        // Move to front of DLL
        if (node->list_node)
            delete_by_value(cache->dll, node->list_node->data);

        node->list_node = insert_front(cache->dll, create_data(node));
        return;
    }

    // Evict least recently used if full
    if (cache->size == cache->capacity)
    {
        Node *lru_node = cache->dll->tail;
        if (lru_node && lru_node->data && lru_node->data->value)
        {
            LRUNode *lru = (LRUNode *)lru_node->data->value;
            tree_map_delete(cache->map, void_ptr_to_int(lru->key));
            delete_by_value(cache->dll, lru_node->data);
            cache->size--;
        }
    }

    // Insert new node
    node = create_lru_node(int_to_void_ptr(key), int_to_void_ptr(value));
    node->list_node = insert_front(cache->dll, create_data(node));
    tree_map_insert(cache->map, key, node);
    cache->size++;

    //printf("Inserting new key=%d, value=%d into DLL and tree map\n", key, value);
}

bool is_full_lru(LRUCache *cache)
{
    return cache->size >= cache->capacity;
}

bool is_empty_lru(LRUCache *cache)
{
    return cache->size == 0;
}

int list_size_lru(LRUCache *cache)
{
    return cache->dll->size;
}

// Print current cache status
void print_lru_cache(LRUCache *cache)
{
    printf("Current Cache Status:\n");
    Node *cur = cache->dll->head;
    while (cur)
    {
        LRUNode *node = (LRUNode *)cur->data->value;
        printf("Key=%d, Value=%d\n", void_ptr_to_int(node->key), void_ptr_to_int(node->value));
        cur = cur->next;
    }
    printf("\n");
}

// Free LRU Cache
void free_lru_cache(LRUCache *cache)
{
    free_list(cache->dll);
    free_tree_map(cache->map);
    free(cache);
}

int main()
{
    LRUCache *cache = create_lru_cache(3);

    printf("Putting key=1, value=10\n");
    lru_cache_put(cache, 1, 10);

    printf("Putting key=2, value=20\n");
    lru_cache_put(cache, 2, 20);

    printf("Putting key=3, value=30\n");
    lru_cache_put(cache, 3, 30);

    printf("Getting key=1: %d\n", void_ptr_to_int(lru_cache_get(cache, 1)));
    printf("Getting key=2: %d\n", void_ptr_to_int(lru_cache_get(cache, 2)));

    printf("Putting key=4, value=40\n");
    lru_cache_put(cache, 4, 40);

    printf("Getting key=3: %d\n", void_ptr_to_int(lru_cache_get(cache, 3)));

    printf("Putting key=5, value=50\n");
    lru_cache_put(cache, 5, 50);

    printf("Getting key=1: %d\n", void_ptr_to_int(lru_cache_get(cache, 1)));
    printf("Getting key=4: %d\n", void_ptr_to_int(lru_cache_get(cache, 4)));
    printf("Getting key=5: %d\n", void_ptr_to_int(lru_cache_get(cache, 5)));

    free_lru_cache(cache);
    return 0;
}