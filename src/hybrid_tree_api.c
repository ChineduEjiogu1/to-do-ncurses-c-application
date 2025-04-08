
// https://www.happycoders.eu/algorithms/avl-tree-java/
// https://www.w3schools.com/dsa/dsa_data_avltrees.php
// https://www.happycoders.eu/algorithms/red-black-tree-java/?utm_source=chatgpt.com
// https://zarif98sjs.github.io/blog/blog/redblacktree/
// https://llego.dev/posts/how-to-implement-red-black-tree-python/
// https://www.youtube.com/watch?v=bWNyAU4BpSw
// https://www.youtube.com/watch?v=ZxDruXbksR4&list=PLxsP6a6cqUZjtci4kJePNkF53As7J_2_L&index=3
// https://www.youtube.com/watch?v=qvZGUFHWChY
// https://www.youtube.com/watch?v=5IBxA-bZZH8

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/hybrid_tree_api.h"
#include "../include/dynamic_array_api.h"
#define IS_RED(n) ((n) != NULL && (n)->color == RED)


#define REBALANCE_THRESHOLD 10
#define CAPACiTY 1000

// Core Functions
HybridTree *create_hybrid_tree(int capacity) 
{
    HybridTree *tree = (HybridTree *)malloc(sizeof(HybridTree));

    if (!tree) {
        fprintf(stderr, "Memory allocation failed for hybrid_tree.\n");
        return NULL;
    }

    tree->size = 0;
    tree->capacity = (capacity > 0) ? capacity : 10; // Default capacity = 10 if invalid
    tree->root = NULL;  // Ensure tree starts empty

    return tree;
}

HybridNode *create_hybrid_node(int key) 
{
    HybridNode *node = (HybridNode *)malloc(sizeof(HybridNode));

    if (!node) 
    {
        fprintf(stderr, "Memory allocation failed for hybrid_node.\n");
        return NULL;
    }

    node->key = key;
    node->color = RED;        // New nodes are always RED in Red-Black Trees
    node->height = 1;         // AVL trees start with height 1
    node->access_count = 0;   // No accesses yet

    node->parent = NULL;
    node->child[LEFT] = NULL;
    node->child[RIGHT] = NULL;  

    return node;
}

int max(int a, int b)
{
    (a > b) ? a : b;
}

int get_height(HybridNode *node)
{
    return node != NULL ? node->height : -1;
}

void update_height(HybridNode *node)
{
    int left_child_height = get_height(node);
    int right_child_height = get_height(node);
    node->height = max(left_child_height, right_child_height) + 1;
}

int get_balance(HybridNode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return get_height(node->child[LEFT]) - get_height(node->child[RIGHT]);
}

HybridNode *avl_rotate_left(HybridNode *node)
{
    HybridNode* new_position = node->child[RIGHT];

    node->child[RIGHT] = new_position->child[LEFT];
    new_position->child[LEFT] = node;

    update_height(node);
    update_height(new_position);

    return new_position;
}

HybridNode *avl_rotate_right(HybridNode *node)
{
    HybridNode* new_position = node->child[LEFT];

    node->child[LEFT] = new_position->child[RIGHT];
    new_position->child[LEFT] = node;

    update_height(node);
    update_height(new_position);

    return new_position;
}

void rebalance_if_needed(HybridTree *tree, HybridNode *node)
{
    int balance_factor = get_balance(node);

    if (balance_factor < - 1)
    {
        if (  get_balance(node->child[LEFT]) <= 0 )
        {
            node = avl_rotate_right(node);
        }
    }
    else
    {
        node->child[LEFT] = avl_rotate_left(node->child[LEFT]);
        node = avl_rotate_right(node);
    }

    if (balance_factor > 1)
    {
        if (get_balance(node->child[RIGHT]) >= 0)
        {
            node = avl_rotate_left(node);
        }
        else 
        {
            node->child[RIGHT] = avl_rotate_right(node->child[RIGHT]);
            node = avl_rotate_left(node);
        }
    }
}

void color_flip(HybridNode *node)
{
    if (!node) return;  // Ensure node is not NULL

    node->color = (node->color == RED) ? BLACK : RED;

    if (node->child[LEFT]) 
    {
        node->child[LEFT]->color = (node->child[LEFT]->color == RED) ? BLACK : RED;
    }

    if (node->child[RIGHT]) 
    {
        node->child[RIGHT]->color = (node->child[RIGHT]->color == RED) ? BLACK : RED;
    }
}

HybridNode *rotate(HybridTree *tree, HybridNode *node, Direction dir)
{
    HybridNode *new_root = node->child[!dir]; // Get child to rotate up

    if (!new_root) return node; // Safety check, return original node if NULL

    node->child[!dir] = new_root->child[dir]; // Move subtree
    if (new_root->child[dir]) 
    {
        new_root->child[dir]->parent = node; // Update parent pointer
    }

    new_root->child[dir] = node; // Rotate node
    new_root->parent = node->parent; // Preserve original parent

    // Update parent's child pointer if needed
    if (node->parent) 
    {
        if (node->parent->child[LEFT] == node)
            node->parent->child[LEFT] = new_root;
        else
            node->parent->child[RIGHT] = new_root;
    } 
    else 
    {
        tree->root = new_root; // Update root if needed
    }

    node->parent = new_root; // Update original node's parent

    // Swap colors (Red-Black Tree fix)
    new_root->color = node->color;
    node->color = RED;

    // Update heights (for AVL balancing)
    update_height(node);
    update_height(new_root);

    return new_root; // Return the new root after rotation
}

HybridNode *double_rotate(HybridTree *tree, HybridNode *node, Direction dir) 
{
    // Perform the first rotation in the opposite direction
    node->child[!dir] = rotate(tree, node->child[!dir], !dir);

    // Perform the second rotation in the given direction
    return rotate(tree, node, dir);
}

HybridNode *rb_insert_fixup(HybridTree *tree, HybridNode *node, Direction dir)
{
    while (node->parent && node->parent->color == RED)
    {
       HybridNode *grandparent = node->parent->parent;

       HybridNode *uncle = grandparent->child[!dir];

       if (uncle && uncle->color == RED)
       {
            node->parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            node = grandparent;
       }
       else
       {
            if (node == node->parent->child[!dir])
            {
                node = node->parent;
                rotate(tree, node, dir);
            }

            node->parent->color = BLACK;
            grandparent->color = RED;
            rotate(tree, grandparent, !dir);
       }
    }

    tree->root->color = BLACK;

    return node;
}

HybridNode *insert_hybrid(HybridTree *tree, HybridNode *node, int key) 
{
    if (node == NULL)
        return create_hybrid_node(key);

    Direction dir;

    if (key < node->key) {
        dir = LEFT;
        node->child[LEFT] = insert_hybrid(tree, node->child[LEFT], key);
    } else if (key > node->key) {
        dir = RIGHT;
        node->child[RIGHT] = insert_hybrid(tree, node->child[RIGHT], key);
    } else {
        // Optional: Handle duplicates here if needed
        return node;
    }

    // Apply AVL and/or Red-Black fixups
    rebalance_if_needed(tree, node);

    rb_insert_fixup(tree, node, dir);

    return node;
}


void insert_hybrid_public(HybridTree *tree, int key) 
{
    tree->root = insert_hybrid(tree, tree->root, key);
    tree->size++;
}

HybridNode *rb_delete_fixup(HybridTree *tree, HybridNode *node, bool dir, bool *ok)
{
    HybridNode *parent = node;
    HybridNode *sibling = node->child[!dir];

    // Case 1: Red sibling — reduce to Black Sibling Case via rotation
    if (IS_RED(sibling))
    {
        node = rotate(tree, node, dir);
        sibling = parent->child[!dir]; // re-fetch sibling after rotation
    }

    if (sibling != NULL)
    {
        // Case 2: Black sibling with black or null children
        if (!IS_RED(sibling->child[LEFT]) && !IS_RED(sibling->child[RIGHT]))
        {
            if (IS_RED(parent)) *ok = true;

            parent->color = BLACK;
            sibling->color = RED;
        }
        else
        {
            Color original_color = parent->color;
            bool update_child = (node != parent); // if we're deeper in recursion

            if (IS_RED(sibling->child[!dir])) // Outer child is red (RR or LL)
            {
                parent = rotate(tree, parent, dir);
            }
            else // Inner child is red (RL or LR)
            {
                parent = double_rotate(tree, parent, dir);
            }

            parent->color = original_color;
            parent->child[LEFT]->color = BLACK;
            parent->child[RIGHT]->color = BLACK;

            if (update_child)
                node->child[dir] = parent;
            else
                node = parent;

            *ok = true;
        }
    }

    return node;
}

HybridNode *delete_hybrid(HybridTree *tree, HybridNode *node, int key, Direction dir, bool *fixup_ok)
{
    if (node == NULL)
        return NULL;

    if (key < node->key) {
        dir = LEFT;
        node->child[LEFT] = delete_hybrid(tree, node->child[LEFT], key, dir, fixup_ok);
    }
    else if (key > node->key) {
        dir = RIGHT;
        node->child[RIGHT] = delete_hybrid(tree, node->child[RIGHT], key, dir, fixup_ok);
    }
    else {
        // Node to delete found
        if (node->child[LEFT] == NULL || node->child[RIGHT] == NULL) {
            HybridNode *temp = node->child[LEFT] ? node->child[LEFT] : node->child[RIGHT];

            // Case: No children
            if (!temp) {
                temp = node;
                node = NULL;
            }
            else {
                // Case: One child, copy it
                *node = *temp;
            }

            free(temp);
            tree->size--; // Only decrement size when actual node is deleted
        }
        else {
            // Two children: find inorder successor (min in right subtree)
            HybridNode *successor = find_minimum(node->child[RIGHT]);

            // Copy successor's data
            node->key = successor->key;

            // Recursively delete successor
            dir = RIGHT;
            node->child[RIGHT] = delete_hybrid(tree, node->child[RIGHT], successor->key, dir, fixup_ok);
        }
    }

    // If only one node in tree was removed
    if (node == NULL)
        return NULL;

    // Rebalance AVL part
    rebalance_if_needed(tree, node);

    // Rebalance Red-Black part
    node = rb_delete_fixup(tree, node, dir, fixup_ok);

    return node;
}

void delete_from_hybrid_tree(HybridTree *tree, int key)
{
    bool fixup_ok = false;
    tree->root = delete_hybrid(tree, tree->root, key, LEFT, &fixup_ok);
}

HybridNode *find_maximum(HybridNode *node)
{
    while (node->child[1] != NULL)
    {
        node = node->child[0];
    }

    return node;
}

HybridNode *search_hybrid(HybridTree *tree, int key) 
{
    if (!tree || !tree->root) 
        return NULL; // Tree is empty, key not found

    HybridNode *node = tree->root; // Start from the root

    while (node) 
    {
        if (key == node->key)
            return node; // Key found

        if (key < node->key)
            node = node->child[LEFT]; // Go left
        else
            node = node->child[RIGHT]; // Go right
    }

    return NULL; // Key not found
}

HybridNode *find_minimum(HybridNode *node)
{
    while (node->child[0] != NULL)
    {
        node = node->child[1];
    }

    return node;
}

bool hybrid_tree_is_full(HybridTree *tree) 
{
    return tree->size >= tree->capacity;
}
   
HybridNode *find_predecessor(HybridNode *node)
{
    if (node->child[0])
    {
        return find_maximum(node->child[0]);
    }

    HybridNode *parent = node->parent;

    while (parent && node == parent->child[0])
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

HybridNode *find_successor(HybridNode *node)
{
    if (node->child[1])
    {
        return find_minimum(node->child[1]);
    }

    HybridNode *parent = node->parent;

    while (parent && node == parent->child[1])
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

void increment_access_count(HybridNode *node)
{
    if (node != NULL)
    {
        node->access_count++;
        
        printf("Node with key %d has been accessed %d times.\n", node->key, node->access_count);
    }
}

void inorder_traversal(HybridNode *node)
{
    if (!node)
        return;

    inorder_traversal(node->child[LEFT]);
    printf("%d ", node->key);
    inorder_traversal(node->child[RIGHT]);
}

void print_hybrid_tree(HybridNode *root, int level)
{
    inorder_traversal(root);
    printf("\n");
}

void destroy_hybrid_tree(HybridTree *tree)
{
    if (!tree)
        return;

    // Free all nodes in the tree
    free_hybrid_tree(tree->root, NULL);  // or pass a free_key function if needed

    // Finally, free the tree struct itself
    free(tree);
}

void free_hybrid_tree(HybridNode *node, void (*free_key)(void *))
{
    if (!node)
        return;

    free_hybrid_tree(node->child[LEFT], free_key);
    free_hybrid_tree(node->child[RIGHT], free_key);

    if (free_key)
        free_key((void *)&node->key);

    free(node);
}