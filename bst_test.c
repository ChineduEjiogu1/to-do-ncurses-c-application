#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst_api.h"
#include "bst_api.c"

// Comparison function for integers
int int_cmp(void *a, void *b) {
    return (*(int *)a - *(int *)b);
}

// Function to copy an integer key
void *copy_int_key(void *key, size_t size) {
    int *new_key = (int *)malloc(sizeof(int));
    if (new_key) {
        *new_key = *(int *)key;
    }
    return new_key;
}

// Function to free an integer key
void free_int_key(void *key) {
    free(key);
}

int main() {
    BST *tree = create_bst(int_cmp, copy_int_key, free_int_key, 10);
    if (!tree) {
        printf("Failed to create BST.\n");
        return 1;
    }

    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    int values[] = {50, 30, 70, 20, 40, 60, 80};  // Dummy data
    
    // Insert elements into BST
    for (int i = 0; i < 7; i++) {
        add_to_bst(tree, &keys[i], &values[i]);
    }

    printf("In-order traversal after insertion:\n");
    traverse_tree(tree);
    printf("\n");

    // Search for a key
    int search_key = 40;
    BSTNode *found = find_bst_node(tree, &search_key);
    if (found) {
        printf("Key %d found in BST.\n", search_key);
    } else {
        printf("Key %d not found in BST.\n", search_key);
    }

    // Remove a key
    int remove_key = 30;
    if (remove_from_bst(tree, &remove_key)) {
        printf("Key %d removed from BST.\n", remove_key);
    } else {
        printf("Key %d not found for deletion.\n", remove_key);
    }

    printf("In-order traversal after deletion:\n");
    traverse_tree(tree);
    printf("\n");

    // Free the BST
    free_bst(tree->root, free_int_key);
    free(tree);

    return 0;
}
