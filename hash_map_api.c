#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "doubly_linked_list.h"
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

    new_hash_map->buckets = (HashMapEntry **)malloc(capacity * sizeof(*new_hash_map->buckets));
    if (!new_hash_map->buckets)
    {
        printf("Memory allocation failed for buckets\n");
        free(new_hash_map);
        return NULL;
    }

    new_hash_map->capacity = capacity;
    new_hash_map->size = 0;
    new_hash_map->load_factor = LOAD_FACTOR_THRESHOLD;

    // Ensure all buckets are initialized to NULL
    for (int i = 0; i < new_hash_map->capacity; i++)
    {
        new_hash_map->buckets[i] = NULL;
    }

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
void resize_hash_map(HashMap *map)
{
    int old_capacity = map->capacity;

    int new_capacity = old_capacity * 2;

    HashMapEntry **new_buckets = (HashMapEntry **)calloc(new_capacity, sizeof(HashMapEntry *));

    if (!new_buckets)
    {
        printf("Memory allocation failed for resizing\n");
        return;
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
void insert_into_hash_map(HashMap *map, int key, Node *node)
{
    int index = hash_function(key, map->capacity);

    HashMapEntry *new_entry = (HashMapEntry *)malloc(sizeof(HashMapEntry));

    if (!new_entry)
    {
        printf("Memory allocation failed for new entry\n");
        return;
    }

    new_entry->key = key;
    new_entry->node = node;
    new_entry->next = map->buckets[index]; // Insert at head of list
    map->buckets[index] = new_entry;
    map->size++;

    // 🔹 **Check resizing after updating size**
    // printf("Checking resize: Size: %d, Capacity: %d, Load Factor: %.2f\n",
    //       map->size, map->capacity, (float)map->size / map->capacity);

    if (needs_resizing(map))
    {
        printf("Resizing hash map...\n");
        resize_hash_map(map);
    }
}


// Delete an entry from the hash map
void delete_from_hash_map(HashMap *map, int key)
{
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
            return;
        }
        previous = current;
        current = current->next;
    }
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

// Main function for testing
int main()
{
    HashMap *hash_map = create_hash_map(10);

    if (!hash_map)
    {
        printf("Failed to create hash map\n");
        return 1;
    }

    printf("Is hash map empty? %s\n", hash_map_is_empty(hash_map) ? "Yes" : "No");

    printf("\nInserting entries to test resizing...\n");
    for (int i = 1; i <= 8; i++)
    {
        insert_into_hash_map(hash_map, i, NULL);
        printf("Inserted key %d\n", i);
    }

    printf("\nFinal hash map contents:\n");
    print_hash_table(hash_map);

    printf("\nHash map statistics:\n");
    printf("Size: %d\n", hash_map_size(hash_map));
    printf("Capacity: %d\n", hash_map->capacity);

    printf("\nFinding entry with key 2...\n");
    HashMapEntry *found_node = find_hash_entry(hash_map, 2);
    printf("Entry found: %s\n", found_node ? "Yes" : "No");

    printf("\nDeleting entry with key 3...\n");
    delete_from_hash_map(hash_map, 3);

    printf("\nHash map after deletion:\n");
    print_hash_table(hash_map);

    free_hash_map(hash_map);

    return 0;
}