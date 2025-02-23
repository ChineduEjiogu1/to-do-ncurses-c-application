#include <stdio.h>
#include <stdlib.h>
#include "avl_api.h"
#include "avl_api.c"

// Helper functions for integer keys and values
int compare_ints(void *a, void *b) {
    return (*(int *)a - *(int *)b);
}

void *copy_int_key(void *key, size_t size) {
    int *new_key = (int *)malloc(size);
    if (new_key) {
        *new_key = *(int *)key;
    }
    return new_key;
}

void free_int_key(void *key) {
    free(key);
}

int main() {
    // Create AVL Tree with capacity 10
    AVL *tree = create_avl(compare_ints, copy_int_key, free_int_key, 10);
    if (!tree) {
        printf("Failed to create AVL tree.\n");
        return 1;
    }

    // Insert elements
    int keys[] = {10, 20, 5, 6, 15, 30, 25};
    for (int i = 0; i < 7; i++) {
        int *value = (int *)malloc(sizeof(int));
        *value = keys[i] * 10; // Just an example value
        if (!add_to_avl(tree, &keys[i], value)) {
            printf("Failed to insert key %d into AVL tree.\n", keys[i]);
        }
    }

    // Display in-order traversal
    printf("In-order traversal after insertion:\n");
    traverse_tree(tree);
    printf("\n");

    // Remove a value (remove key 20)
    int key_to_remove = 20;
    int value_to_remove = 200;
    if (remove_from_avl(tree, &key_to_remove, &value_to_remove)) {
        printf("Removed value %d from key %d.\n", value_to_remove, key_to_remove);
    } else {
        printf("Failed to remove value %d from key %d.\n", value_to_remove, key_to_remove);
    }

    // Display in-order traversal after deletion
    printf("In-order traversal after deletion:\n");
    traverse_tree(tree);
    printf("\n");

    // Cleanup memory
    free_avl(tree->root, free_int_key);
    free(tree);

    return 0;
}
