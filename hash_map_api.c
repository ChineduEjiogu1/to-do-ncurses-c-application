#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "doubly_linked_list.h"
#include "hash_map.h"

#define LOAD_FACTOR_THRESHOLD 0.75

// Create a new hash map
HashMap *create_hash_map(int capacity) 
{
    if (capacity <= 0) 
    {
        capacity = MAX_SIZE;
    }

    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    if (!map) 
    {
        printf("Memory allocation failed for hash map\n");
        return NULL;
    }

    map->capacity = capacity;
    map->size = 0;
    map->load_factor = 0.0;
    map->buckets = (HashMapEntry **)calloc(capacity, sizeof(HashMapEntry *));
    
    if (!map->buckets) 
    {
        free(map);
        printf("Memory allocation failed for buckets\n");
        return NULL;
    }

    return map;
}


// Hash function using multiplicative hashing
int hash_function(HashMap *map, int key)
{
    return (unsigned int)(key * 2654435761U) % map->capacity;
}

// Check if hash map is empty
bool hash_map_is_empty(HashMap *map) 
{
    return map->size == 0;
}

// Get the size of the hash map
int hash_map_size(HashMap *map) 
{
    return map->size;
}

// Check if resizing is needed
bool needs_resizing(HashMap *map) 
{
    return map->load_factor >= LOAD_FACTOR_THRESHOLD;
}

// Insert into hash map (with resizing)
void insert_into_hash_map(HashMap *map, int key, Node *node)
{
    if (needs_resizing(map))
    {
        resize_hash_map(map, map->capacity * 2);
    }

    int index = hash_function(map, key);
    HashMapEntry *entry = map->buckets[index];

    while (entry)
    {
        if (entry->key == key)
        {
            entry->node = node; // ✅ Update existing entry
            return;
        }
        entry = entry->next;
    }

    // Insert new entry if key was not found
    HashMapEntry *new_entry = (HashMapEntry *)malloc(sizeof(HashMapEntry));
    if (!new_entry)
    {
        printf("Memory allocation failed for hash map entry\n");
        return;
    }

    new_entry->key = key;
    new_entry->node = node;
    new_entry->next = map->buckets[index]; // Insert at head (chaining)
    map->buckets[index] = new_entry;
    map->size++;

    // Update load factor
    map->load_factor = (float)map->size / map->capacity;
}

// Delete from hash map
void delete_from_hash_map(HashMap *map, int key)
{
    int index = hash_function(map, key);
    HashMapEntry *entry = map->buckets[index];
    HashMapEntry *prev = NULL;

    while (entry)
    {
        if (entry->key == key)
        {
            if (prev)
            {
                prev->next = entry->next;
            }
            else
            {
                map->buckets[index] = entry->next;
            }

            // If Node is dynamically allocated, free it
            free(entry);
            map->size--;

            // Update load factor
            map->load_factor = (float)map->size / map->capacity;
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

// Find an entry in the hash map
Node *find_hash_map(HashMap *map, int key)
{
    int index = hash_function(map, key);
    HashMapEntry *entry = map->buckets[index];

    while (entry)
    {
        if (entry->key == key)
        {
            return entry->node;
        }
        entry = entry->next;
    }

    return NULL;
}

// Resize the hash map
void resize_hash_map(HashMap *map, int new_capacity)
{
    if (new_capacity <= map->size)
    {
        printf("New capacity must be greater than current size.\n");
        return;
    }

    HashMapEntry **new_buckets = (HashMapEntry **)calloc(new_capacity, sizeof(HashMapEntry *));
    if (!new_buckets)
    {
        printf("Memory allocation failed for new hash map buckets\n");
        return;
    }

    int old_capacity = map->capacity;
    HashMapEntry **old_buckets = map->buckets;

    map->capacity = new_capacity;
    map->buckets = new_buckets;
    map->size = 0;

    for (int i = 0; i < old_capacity; i++)
    {
        HashMapEntry *entry = old_buckets[i];
        while (entry)
        {
            int new_index = hash_function(map, entry->key);

            HashMapEntry *next = entry->next;
            entry->next = new_buckets[new_index];
            new_buckets[new_index] = entry;

            entry = next;
            map->size++;
        }
    }

    free(old_buckets);
    map->load_factor = (float)map->size / map->capacity;
}

// Free the hash map
void free_hash_map(HashMap *map)
{
    if (!map)
    {
        return;
    }

    for (int i = 0; i < map->capacity; i++)
    {
        HashMapEntry *entry = map->buckets[i];
        while (entry)
        {
            HashMapEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }

    free(map->buckets);
    free(map);
}
