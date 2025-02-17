#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"
#include "bst.c"

// Function to compare integer keys
int compare_keys(void *a, void *b) 
{
    return (*(int *)a - *(int *)b);
}

// Function to free integer keys
void free_int_key(void *key) 
{
    free(key);
}

// Function to free integer data
void free_int_data(void *data) 
{
    free(data);
}

int main() 
{
    // Create BST
    BST *tree = create_tree(compare_keys);
    if (!tree) 
    {
        return 1;
    }

    // Insert some values
    for (int i = 5; i > 0; i--) 
    {
        int *key = (int *)malloc(sizeof(int));
        int *data = (int *)malloc(sizeof(int));
        *key = i;
        *data = i * 10;
        add_to_bst(tree, key, data);
    }

    printf("Inorder Traversal of BST: ");
    inorder_traversal(tree->root);
    printf("\n");

    // Delete a node
    int delete_key = 3;
    printf("Deleting key: %d\n", delete_key);
    tree->root = delete_bst_node(tree->root, &delete_key, free_int_data, free_int_key, compare_keys, copy_key, copy_data);

    printf("Inorder Traversal after Deletion: ");
    inorder_traversal(tree->root);
    printf("\n");

    // Free tree
    free_tree(tree->root, free_int_data, free_int_key);
    free(tree);
    return 0;
}