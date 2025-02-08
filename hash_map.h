#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "doubly_linked_list.h"

#define MAX_SIZE 1000

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

int hash_function(HashMap *map , int key);

bool needs_resizing(HashMap *map);

void insert_into_hash_map(HashMap *map, int key, Node *node);

void delete_from_hash_map(HashMap *map, int key);

Node *find_hash_map(HashMap *map, int key);

int hash_map_size(HashMap* map);

bool hash_map_is_empty(HashMap *map);

void free_hash_map(HashMap *map);

#endif