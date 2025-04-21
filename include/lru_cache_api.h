#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "../include/lru_cache_api.h"
#include "../include/doubly_linked_list.h"
#include "../include/tree_map_api.h"

// LRU Cache Node
typedef struct LRUNode {
    void *key;
    void *value;
} LRUNode;

typedef struct LRUCache {
    int capacity;
    int size;
    DoublyLinkedList *dll;        // Stores LRU order (front = most recent)
    HashMapWithTree *map;                 // Maps key -> DLL node
} LRUCache;


// Core Functions
LRUCache *create_lru_cache(int capacity);
void insert_new(LRUCache *cache, void *key, void *value);
bool lru_contains(LRUCache *cache, void *key);

void evict_lru(LRUCache *cache);
void insert_new(LRUCache *cache, void *key, void *value);
void *lru_get(LRUCache *cache, void *key);

void *lru_get(LRUCache *cache, void *key);
void lru_put(LRUCache *cache, void *key, void *value);

bool lru_contains(LRUCache *cache, void *key);
int lru_size(LRUCache *cache);
void lru_print(LRUCache *cache);
bool is_full_lru(LRUCache *cache);
bool is_empty_lru(LRUCache *cache);
int list_size_lru(LRUCache *cache);

void free_lru_cache(LRUCache *cache);
#endif // LRU_CACHE_H
