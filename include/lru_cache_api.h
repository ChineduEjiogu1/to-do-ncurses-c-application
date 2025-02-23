#ifndef LRU_CACHE_H
#define LRU_CACHE_H

#include "doubly_linked_list.h"
#include "tree_map_api.h"  // To connect with TreeMap
#include "hybrid_tree_api.h"  // To connect with HybridTree

// LRU Cache Node
typedef struct LRUNode {
    int key;
    Node *data; // Pointer to task or appointment data
    struct LRUNode *prev;
    struct LRUNode *next;
} LRUNode;

// LRU Cache Structure
typedef struct LRUCache {
    int capacity;
    int size;
    LRUNode *head;
    LRUNode *tail;
    TreeMap *map; // Quick access using the existing TreeMap
} LRUCache;

// Core Functions
LRUCache *create_lru_cache(int capacity);
void free_lru_cache(LRUCache *cache);

// Cache Operations
void access_lru_cache(LRUCache *cache, int key, Node *data);
Node *get_from_lru_cache(LRUCache *cache, int key);
void remove_lru_node(LRUCache *cache, LRUNode *node);
void move_to_front(LRUCache *cache, LRUNode *node);

#endif // LRU_CACHE_H
