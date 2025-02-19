#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "hash_map_api.h"
#include "hash_map_api.c"
#include "doubly_linked_list.h"
#include "doubly_linked_list_api.c"
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

    DoublyLinkedList *dll = create_list(100); // Create a list

    for (int i = 1; i <= 8; i++)
    {
        Data *data = (Data *)malloc(sizeof(Data));

        if (!data)
        {
            printf("Error: Memory allocation failed for data\n");
            continue;
        }
        data->value = malloc(20); // Allocate space for a value (optional)
        if (!data->value)
        {
            printf("Error: Memory allocation failed for data->value\n");
            free(data);
            continue;
        }

        Node *new_node = insert_back(dll, data); // Use an actual DLL
        if (!new_node)
        {
            printf("Error: Failed to create node for key %d\n", i);
            free(data->value);
            free(data);
            continue;
        }

        if (!insert_into_hash_map(hash_map, i, new_node))
        {
            printf("Failed to insert key %d\n", i);
        }
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

    free_list(dll); // Free DLL
    free_hash_map(hash_map);       // Free hash map
    return 0;
}