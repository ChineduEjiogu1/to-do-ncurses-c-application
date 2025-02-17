#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "bst.h"
#include "doubly_linked_list_api.c"

BST *create_tree(int (*cmp)(void *, void *))
{
    BST *new_tree = (BST *)malloc(sizeof(BST));

    if (!new_tree)
    {
        printf("Memory allocation failed for tree\n");
        return NULL;
    }

    new_tree->root = NULL;

    if (!cmp)
    {
        printf("Error: Comparison function is NULL\n");
        free(new_tree);
        return NULL;
    }

    new_tree->cmp = cmp;

    return new_tree;
}

void add_to_bst(BST *tree, void *key, void *data) 
{
    tree->root = insert_into_bst(tree->root, key, data, tree->cmp);
}

void *copy_key(void *key) 
{
    char *new_key = (char *)malloc(strlen((char *)key) + 1);

    if (new_key)
    {
        strcpy(new_key, (char *)key);
    }

    return new_key;
}

void *copy_data(void *data) 
{
    int *new_data = (int *)malloc(sizeof(int));

    if (new_data)
    {
        *new_data = *(int *)data;
    }

    return new_data;
}

BSTNode *find_min(BSTNode *node)
{
    if (node == NULL)
    {
        return NULL;
    }
    else if (node->left != NULL)
    {
        return find_min(node->left);
    }
    
    return node;
}

BSTNode *find_bst_node(BSTNode *node, void *key, int (*cmp)(void *, void *))
{
    if (!node)
    {
        return NULL;
    }

    if (cmp(key, node->key) == 0)
    {
        return node;
    }
    else if (cmp(key, node->key) < 0)
    {
        return find_bst_node(node->left, key, cmp);
    }
    else
    {
        return find_bst_node(node->right, key, cmp);
    }
}

BSTNode *insert_into_bst(BSTNode *root, void *key, void *data, int (*cmp)(void *, void *)) 
{
    if (!root) 
    {
        BSTNode *new_node = (BSTNode *)malloc(sizeof(BSTNode));
        if (!new_node)
        {
            printf("Memory allocation failed for new node\n");
            return NULL;
        }
        new_node->key = key;
        new_node->data = data;
        new_node->left = new_node->right = NULL;
        return new_node;
    }

    if (cmp(key, root->key) < 0) 
    {
        root->left = insert_into_bst(root->left, key, data, cmp);
    } 
    else if (cmp(key, root->key) > 0) 
    {
        root->right = insert_into_bst(root->right, key, data, cmp);
    } 
    else 
    {
        // Key already exists, update data
        root->data = data;
    }

    return root;
}

BSTNode *delete_bst_node(BSTNode *node, void *key, void (*free_data)(void *), void (*free_key)(void *), 
                         int (*cmp)(void *, void *), void *(*copy_key)(void *), void *(*copy_data)(void *)) 
{
    if (!node) 
    {
        return NULL;
    }

    if (cmp(key, node->key) < 0) 
    {
        node->left = delete_bst_node(node->left, key, free_data, free_key, cmp, copy_key, copy_data);
    } 
    else if (cmp(key, node->key) > 0) 
    {
        node->right = delete_bst_node(node->right, key, free_data, free_key, cmp, copy_key, copy_data);
    } 
    else 
    {
        if (!node->left) 
        {
            BSTNode *temp = node->right;
            free_key(node->key);
            free_data(node->data);
            free(node);
            return temp;
        } 
        else if (!node->right) 
        {
            BSTNode *temp = node->left;
            free_key(node->key);
            free_data(node->data);
            free(node);
            return temp;
        }

        // Find the minimum node in the right subtree
        BSTNode *temp = find_min(node->right);

        // Create deep copies of key and data
        void *new_key = copy_key(temp->key);
        void *new_data = copy_data(temp->data);

        // Free old key and data before replacing them
        free_key(node->key);
        free_data(node->data);

        // Assign new deep copies
        node->key = new_key;
        node->data = new_data;

        // Recursively delete the successor node
        node->right = delete_bst_node(node->right, temp->key, free_data, free_key, cmp, copy_key, copy_data);
    }

    return node;
}

void inorder_traversal(BSTNode *node) 
{
    if (node == NULL)
    {
        return;
    } 
    
    inorder_traversal(node->left);  // Visit left subtree
    printf("%d ", *(int *)node->key);  // Process current node
    inorder_traversal(node->right); // Visit right subtree
}


void free_tree(BSTNode *root, void (*free_data)(void *), void (*free_key)(void *))
{
    if (!root)
    {
        return;
    }

    // Recursively free left and right subtrees
    free_tree(root->left, free_data, free_key);
    free_tree(root->right, free_data, free_key);

    // Free the key if a custom free function is provided
    if (free_key && root->key)
    {
        free_key(root->key);
    }

    // Free the data if a custom free function is provided
    if (free_data && root->data)
    {
        free_data(root->data);
    }

    // Free the node itself
    free(root);
}
