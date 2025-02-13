#ifndef HASH_MAP_H
#define HASH_MAP_H
#include "doubly_linked_list.h"

#define MAX_TABLE_SIZE 1000

typedef struct HashMapEntry
{
    int key;
    Node *node;
    struct HashMapEntry *next;
}HashMapEntry;

typedef struct HashMap 
{
    int capacity;
    int size;
    float load_factor;
    HashMapEntry **buckets;
} HashMap;


HashMap *create_hash_map(int capacity);

HashMapEntry *create_hash_entry(Node *node, int key);

int hash_function(int capacity, int key);

bool needs_resizing(HashMap *map);

void resize_hash_map(HashMap *map);

void insert_into_hash_map(HashMap *map, int key, Node *node);

void delete_from_hash_map(HashMap *map, int key);

HashMapEntry *find_hash_entry(HashMap *map, int key);

int hash_map_size(HashMap* map);

bool hash_map_is_empty(HashMap *map);

void print_hash_table(HashMap *map);

void free_hash_map(HashMap *map);

#endif