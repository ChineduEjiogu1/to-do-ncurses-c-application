#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "bst_api.h"
#include "doubly_linked_list_api.c"

BST *create_bst(int (*cmp)(void *, void *), 
    void *(*copy_key)(void *, size_t), void (*free_key)(void *), int capacity)
{
    BST *tree = (BST *)malloc(sizeof(BST));

    if (!tree)
    {
        printf("Memory allocation failed for creating tree \n");
        return NULL;
    }

    tree->root = NULL;
    tree->cmp = cmp;
    tree->copy_key = copy_key;
    tree->free_key = free_key;
    tree->size = 0;
    tree->capacity = capacity;

    return tree;
}

BSTNode *create_bst_node(BST *tree, void *key, void *data)
{
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));

    if (!node)
    {
        printf("Memory allocation failed for creating node \n");
        return NULL;
    }

    node->data = data;
    node->key = tree->copy_key(key, sizeof(key));
    node->left = node->right = NULL; 

    return node;
}

bool is_bst_empty(BST *tree)
{
    return tree->root == NULL;
}

int get_bst_size(BST *tree)
{
    return tree ? tree->size : 0;
}

bool to_left(BST *tree, void *new_key, void *current_key)
{
    return tree->cmp(new_key, current_key) < 0;
}

bool to_right(BST *tree, void *new_key, void *current_key)
{
    return tree->cmp(new_key, current_key) > 0;
}

bool is_bst_full(BST *tree)
{
    return tree->size >= tree->capacity;
}

BSTNode *find_bst_node(BST *tree, void *key)
{
    return find_bst(tree, tree->root, key);
}

BSTNode *find_min(BSTNode *node)
{
    if (!node || !node->left) 
    {
        return node;
    }

    return find_min(node->left);    
}

BSTNode *find_min_bst(BST *tree)
{
    if (!tree || !tree->root)
    {
        return NULL;
    }
    return find_min(tree->root);
}

BSTNode *find_bst(BST *tree, BSTNode *node, void *key)
{
    if (!node)
    {
        return NULL;
    }

    if (!to_left(tree, node->key, key) && !to_right(tree, node->key, key))
    {
        return node;
    }
    else if (to_left(tree, node->key, key))
    {
        return find_bst(tree, node->left, key);
    }
    else
    {
        return find_bst(tree, node->right, key);
    } 
}

bool insert_into_bst(BSTNode **node, BST *tree, void *key, void *data)
{
    if (*node == NULL)
    {
        *node = create_bst_node(tree, key, data);
        if (*node == NULL)
        {
            return false;
        }
        tree->size++;
        return true;
    }

    if (to_left(tree, (*node)->key, key))
    {
        return insert_into_bst(&(*node)->left, tree, key, data);
    }
    else if (to_right(tree, (*node)->key, key))
    {
        return insert_into_bst(&(*node)->right, tree, key, data);
    }
    else
    {
        (*node)->data = data;  
        return false;
    }
}

bool add_to_bst(BST *tree, void *key, void *data)
{
    return insert_into_bst(&tree->root, tree, key, data);
}

bool delete_from_bst(BSTNode **node, BST *tree, void *key) 
{
    if (!(*node)) 
    {
        return false;
    }

    if (to_left(tree, (*node)->key, key)) 
    {
        return delete_from_bst(&((*node)->left), tree, key);
    }
    else if (to_right(tree, (*node)->key, key)) 
    {
        return delete_from_bst(&((*node)->right), tree, key);
    }
    else 
    {
        if (!(*node)->left) 
        {
            BSTNode *temp = (*node)->right;
            free(*node);
            *node = temp;
        } 
        else if (!(*node)->right) 
        {
            BSTNode *temp = (*node)->left;
            free(*node);
            *node = temp;
        } 
        else 
        {
            BSTNode *min_node = find_min((*node)->right);
            (*node)->key = min_node->key;
            (*node)->data = min_node->data;
            delete_from_bst(&((*node)->right), tree, min_node->key);
        }
        tree->size--;  
        return true;
    }
}

bool remove_from_bst(BST *tree, void *key)
{
    return delete_from_bst(&tree->root, tree, key);
}

void inorder_traversal(BSTNode *node)
{
    if (!node)
    {
        return;
    }

    inorder_traversal(node->left);
    printf("%d ", *(int *)node->data);
    inorder_traversal(node->right);
}

void traverse_tree(BST *tree)
{
    if (!tree || tree->root == NULL)
    {
        return;
    }

    inorder_traversal(tree->root);
}

void free_bst(BSTNode *node, void (*free_key)(void *))
{
    if (!node)
    {
        return;
    }
    
    free_bst(node->left, free_key);
    free_bst(node->right, free_key);

    if (free_key)
    {
        free_key(node->key);
    }

    free(node->data);
    free(node);
}