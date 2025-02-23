#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "avl_api.h"

// ---------------- AVL Value List Functions ----------------

AVLValueList *create_value_list(int capacity)
{
    AVLValueList *list = (AVLValueList *)malloc(sizeof(AVLValueList));
    if (!list)
    {
        printf("Memory allocation failed for AVLValueList.\n");
        return NULL;
    }
    list->values = (void **)malloc(sizeof(void *) * capacity); // Use provided capacity
    list->size = 0;
    list->capacity = capacity;
    return list;
}

bool add_value_to_list(AVLValueList *list, void *data)
{
    if (!list)
        return false;

    if (list->size >= list->capacity)
    {
        size_t new_capacity = list->capacity * 2;
        void **new_values = (void **)realloc(list->values, sizeof(void *) * new_capacity);
        if (!new_values)
        {
            printf("Failed to expand AVLValueList.\n");
            return false;
        }
        list->values = new_values;
        list->capacity = new_capacity;
    }

    list->values[list->size++] = data;
    return true;
}

AVLValueList *copy_value_list(AVLValueList *source) {
    if (!source) return NULL;

    AVLValueList *copy = create_value_list(source->capacity);
    if (!copy) return NULL;

    for (size_t i = 0; i < source->size; i++) {
        copy->values[i] = source->values[i]; // Shallow copy (just copying pointers)
    }
    copy->size = source->size;

    return copy;
}

bool remove_value_from_list(AVLValueList *list, void *data)
{
    if (!list || list->size == 0)
        return false;

    for (size_t i = 0; i < list->size; i++)
    {
        if (*(int *)list->values[i] == *(int *)data) // Compare actual int values
        {
            for (size_t j = i; j < list->size - 1; j++)
            {
                list->values[j] = list->values[j + 1];
            }
            list->size--;
            return true;
        }
    }
    return false;
}

void free_value_list(AVLValueList *list)
{
    if (!list)
        return;
    free(list->values);
    free(list);
}

// ---------------- AVL Tree Functions ----------------

AVL *create_avl(int (*cmp)(void *, void *), void *(*copy_key)(void *, size_t), void (*free_key)(void *), int capacity)
{
    AVL *tree = (AVL *)malloc(sizeof(AVL));
    if (!tree)
    {
        printf("Memory allocation failed for AVL tree.\n");
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

AVLNode *create_avl_node(AVL *tree, void *key, void *data)
{
    AVLNode *node = (AVLNode *)malloc(sizeof(AVLNode));
    if (!node)
    {
        printf("Memory allocation failed for AVLNode.\n");
        return NULL;
    }

    node->key = tree->copy_key(key, sizeof(*(int *)key)); // Assuming key is an int
    node->list = create_value_list(2);
    if (!node->list || !add_value_to_list(node->list, data))
    {
        free(node);
        return NULL;
    }
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

bool is_avl_empty(AVL *tree)
{
    return tree->root == NULL;
}

int get_avl_size(AVL *tree)
{
    return tree ? tree->size : 0;
}

bool is_avl_full(AVL *tree)
{
    return tree->size >= tree->capacity;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

bool to_left(AVL *tree, void *new_key, void *current_key)
{
    return tree->cmp(new_key, current_key) < 0;
}

bool to_right(AVL *tree, void *new_key, void *current_key)
{
    return tree->cmp(new_key, current_key) > 0;
}

// --- Find Minimum Node in Subtree ---
AVLNode *find_min(AVLNode *node)
{
    if (!node)
        return NULL;

    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

// --- Find Node by Key ---
AVLNode *find_avl(AVL *tree, AVLNode *node, void *key)
{
    if (!node)
        return NULL;

    int cmp_result = tree->cmp(key, node->key);

    if (cmp_result == 0)
    {
        return node;
    }
    else if (cmp_result < 0)
    {
        return find_avl(tree, node->left, key);
    }
    else
    {
        return find_avl(tree, node->right, key);
    }
}

// ---------------- AVL Tree Balancing ----------------

int get_height(AVLNode *node)
{
    return node ? node->height : 0;
}

int get_balance_factor(AVLNode *node)
{
    return node ? get_height(node->left) - get_height(node->right) : 0;
}

// Rotate Left with Parent Updates
AVLNode *rotate_left(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    y->left = x;
    x->right = T2;

    y->parent = x->parent;
    x->parent = y;
    if (T2)
        T2->parent = x;

    x->height = 1 + max(get_height(x->left), get_height(x->right));
    y->height = 1 + max(get_height(y->left), get_height(y->right));

    return y;
}

// Rotate Right with Parent Updates
AVLNode *rotate_right(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    x->right = y;
    y->left = T2;

    x->parent = y->parent;
    y->parent = x;
    if (T2)
        T2->parent = y;

    y->height = 1 + max(get_height(y->left), get_height(y->right));
    x->height = 1 + max(get_height(x->left), get_height(x->right));

    return x;
}

// Balance AVL Node
AVLNode *balance_avl(AVLNode *node)
{
    node->height = 1 + max(get_height(node->left), get_height(node->right));

    int balance = get_balance_factor(node);

    // Left Heavy
    if (balance > 1)
    {
        if (get_balance_factor(node->left) < 0)
            node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // Right Heavy
    if (balance < -1)
    {
        if (get_balance_factor(node->right) > 0)
            node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

// ---------------- AVL Tree Insert and Delete ----------------

bool insert_into_avl(AVLNode **node, AVL *tree, void *key, void *data)
{
    if (*node == NULL)
    {
        *node = create_avl_node(tree, key, data);
        if (*node == NULL)
            return false;
        tree->size++;
        return true;
    }

    if (to_left(tree, key, (*node)->key))
    {
        if (!insert_into_avl(&(*node)->left, tree, key, data))
            return false;
    }
    else if (to_right(tree, key, (*node)->key))
    {
        if (!insert_into_avl(&(*node)->right, tree, key, data))
            return false;
    }
    else
    {
        return add_value_to_list((*node)->list, data);
    }

    *node = balance_avl(*node);
    return true;
}

bool delete_from_avl(AVLNode **node, AVL *tree, void *key, void *value)
{
    if (!(*node))
        return false; // Node not found

    // Traverse the AVL tree to find the node
    if (to_left(tree, key, (*node)->key))
    {
        return delete_from_avl(&((*node)->left), tree, key, value);
    }
    else if (to_right(tree, key, (*node)->key))
    {
        return delete_from_avl(&((*node)->right), tree, key, value);
    }
    else
    {
        // Found the node; remove the value from its dynamic array
        if (!remove_value_from_list((*node)->list, value))
            return false;

        // If the dynamic array is empty, we need to remove the AVL node itself
        if ((*node)->list->size == 0)
        {
            // Case 1: No children
            if (!(*node)->left && !(*node)->right)
            {
                free_value_list((*node)->list);
                tree->free_key((*node)->key);
                free(*node);
                *node = NULL;
            }
            // Case 2: One child
            else if (!(*node)->left || !(*node)->right)
            {
                AVLNode *temp = (*node)->left ? (*node)->left : (*node)->right;
                free_value_list((*node)->list);
                tree->free_key((*node)->key);
                **node = *temp;
                free(temp);
            }
            // Case 3: Two children
            else
            {
                AVLNode *successor = (*node)->right;
                while (successor->left != NULL)
                    successor = successor->left;

                // Copy successor's key and value list
                tree->free_key((*node)->key);
                (*node)->key = tree->copy_key(successor->key, sizeof(*(int *)successor->key));

                free_value_list((*node)->list);
                (*node)->list = copy_value_list(successor->list); // Use copy_value_list here

                // Recursively delete the successor node
                delete_from_avl(&((*node)->right), tree, successor->key, successor->list->values[0]);
            }
            tree->size--;
        }
    }

    // Rebalance the tree after deletion
    if (*node)
        *node = balance_avl(*node);

    return true;
}

bool add_to_avl(AVL *tree, void *key, void *data)
{
    return insert_into_avl(&tree->root, tree, key, data);
}

bool remove_from_avl(AVL *tree, void *key, void *value)
{
    return delete_from_avl(&tree->root, tree, key, value);
}

// ---------------- AVL Tree Traversal and Cleanup ----------------

void inorder_traversal(AVLNode *node)
{
    if (!node)
        return;
    inorder_traversal(node->left);
    printf("%d ", *(int *)node->key);
    inorder_traversal(node->right);
}

void traverse_tree(AVL *tree)
{
    inorder_traversal(tree->root);
}

void free_avl(AVLNode *node, void (*free_key)(void *))
{
    if (!node)
        return;
    free_avl(node->left, free_key);
    free_avl(node->right, free_key);
    free_value_list(node->list);
    free_key(node->key);
    free(node);
}
