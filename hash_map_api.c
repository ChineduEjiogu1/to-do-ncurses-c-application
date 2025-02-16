#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "hash_map.h"

#define LOAD_FACTOR_THRESHOLD 0.75

HashMap *create_hash_map(int capacity)
{
    HashMap *new_hash_map = (HashMap *)malloc(sizeof(HashMap));

    if (!new_hash_map)
    {
        printf("Memory allocation failed for hash table\n");
        return NULL;
    }

    new_hash_map->buckets = (HashMapEntry **)calloc(capacity, sizeof(*new_hash_map->buckets));
    
    if (!new_hash_map->buckets)
    {
        printf("Memory allocation failed for buckets\n");
        free(new_hash_map);
        return NULL;
    }

    new_hash_map->capacity = capacity;
    new_hash_map->size = 0;
    new_hash_map->load_factor = LOAD_FACTOR_THRESHOLD;

    return new_hash_map;
}

HashMapEntry *create_hash_entry(Node *node, int key)
{
    HashMapEntry *new_entry = (HashMapEntry *)malloc(sizeof(HashMapEntry));

    if (!new_entry)
    {
        printf("Memory allocation failed for hash entry\n");
        return NULL;
    }

    new_entry->key = key;
    new_entry->node = node;
    new_entry->next = NULL;

    return new_entry;
}

int hash_function(int key, int capacity)
{
    return abs((key * 31) % capacity);
}

bool hash_map_is_empty(HashMap *map)
{
    return map->capacity == 0;
}

int hash_map_size(HashMap *map)
{
    return map->size;
}

bool needs_resizing(HashMap *map)
{
    return (float)map->size / map->capacity >= LOAD_FACTOR_THRESHOLD;
}

// Resize the hash map
bool resize_hash_map(HashMap *map)
{
    int old_capacity = map->capacity;
    int new_capacity = old_capacity * 2;

    HashMapEntry **new_buckets = (HashMapEntry **)calloc(new_capacity, sizeof(HashMapEntry *));
    if (!new_buckets)
    {
        printf("Memory allocation failed for resizing\n");
        return false;
    }

    // Rehash all elements
    for (int i = 0; i < old_capacity; i++)
    {
        HashMapEntry *entry = map->buckets[i];

        while (entry)
        {
            HashMapEntry *next_entry = entry->next;
            int index = hash_function(entry->key, new_capacity);
            entry->next = new_buckets[index];
            new_buckets[index] = entry;

            entry = next_entry;
        }
    }

    free(map->buckets);
    map->buckets = new_buckets;
    map->capacity = new_capacity;

    return true; // Success
}

// Find an entry in the hash map
HashMapEntry *find_hash_entry(HashMap *map, int key)
{
    int index = hash_function(key, map->capacity); // Ensure correct hashing
    HashMapEntry *entry = map->buckets[index];

    while (entry)
    {
        if (entry->key == key) 
        {
            return entry; 
        }
        entry = entry->next; 
    }

    return NULL; 
}

// Insert into the hash map
bool insert_into_hash_map(HashMap *map, int key, Node *node)
{
    if (!map)
    {
        printf("Error: Hash map is NULL\n");
        return false;
    }

    if (!node)
    {
        printf("Error: Node is NULL\n");
        return false;
    }

    int index = hash_function(key, map->capacity);

    if (index < 0 || index >= map->capacity)
    {
        printf("Error: Invalid index calculated by hash function (%d)\n", index);
        return false;
    }

    HashMapEntry *new_entry = (HashMapEntry *)malloc(sizeof(HashMapEntry));

    if (!new_entry)
    {
        printf("Memory allocation failed for new entry\n");
        return false;
    }

    new_entry->key = key;
    new_entry->node = node;
    new_entry->next = map->buckets[index]; // Insert at head of list
    map->buckets[index] = new_entry;
    map->size++;

    printf("Inserted key %d at index %d\n", key, index);

    // Resize if needed
    if (needs_resizing(map))
    {
        printf("Resizing hash map...\n");
        if (!resize_hash_map(map))
        {
            printf("Error: Failed to resize hash map\n");
            return false;
        }
    }

    return true; // Success
}

// Delete an entry from the hash map
bool delete_from_hash_map(HashMap *map, int key)
{
    if (!map)
    {
        printf("Error: Hash map is NULL\n");
        return false;
    }

    int index = hash_function(key, map->capacity);

    HashMapEntry *current = map->buckets[index];
    HashMapEntry *previous = NULL;

    while (current)
    {
        if (current->key == key)
        {
            if (previous)
            {
                previous->next = current->next;
            }
            else
            {
                map->buckets[index] = current->next;
            }

            free(current);
            map->size--;
            return true; // Successfully deleted
        }
        previous = current;
        current = current->next;
    }

    printf("Error: Key %d not found in hash map\n", key);
    return false; // Key not found
}


void print_hash_table(HashMap *map)
{
    if (!map)
    {
        printf("Hash map is NULL\n");
        return;
    }

    for (int i = 0; i < map->capacity; i++)
    {
        printf("Bucket %d:", i);

        HashMapEntry *current = map->buckets[i];

        if (!current)
        {
            printf(" empty");
        }

        while (current)
        {
            printf(" -> Key: %d", current->key);
            current = current->next;
        }

        printf(" -> NULL\n"); // Marks the end of the chain
    }

    printf("End of hash map\n");
}

void free_hash_map(HashMap *map)
{
    if (!map)
    {
        return;
    }

    for (int i = 0; i < map->capacity; i++) // Use map->capacity
    {
        HashMapEntry *current = map->buckets[i];

        while (current)
        {
            HashMapEntry *temp = current->next;
            free(current);
            current = temp;
        }
    }

    free(map->buckets); // Free the array of bucket pointers
    free(map);          // Free the HashMap structure itself
}