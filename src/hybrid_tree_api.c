
// https://www.happycoders.eu/algorithms/avl-tree-java/
// https://www.w3schools.com/dsa/dsa_data_avltrees.php
// https://www.happycoders.eu/algorithms/red-black-tree-java/?utm_source=chatgpt.com
// https://zarif98sjs.github.io/blog/blog/redblacktree/
// https://llego.dev/posts/how-to-implement-red-black-tree-python/

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/hybrid_tree_api.h"
#include "../include/dynamic_array_api.h"

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


// Node* INSERT_FIX_UP(Node *node, bool dir)
// {
//     if (red(node->child[dir])) {
//         // Case 1: If the child in the 'dir' direction is red, fix the violation
//         if (red(node->child[!dir])) {
//             // Case 1.1: If the sibling child is also red, perform a color flip
//             if (red(node->child[dir]->child[dir]) || red(node->child[dir]->child[!dir])) {
//                 colorFlip(node);
//             }
//         } else {
//             // Case 2: The sibling child is not red
//             if (red(node->child[dir]->child[dir])) {
//                 // Case 2.1: If the left or right child of the 'dir' child is red, rotate
//                 node = rotate(node, !dir);
//             } else if (red(node->child[dir]->child[!dir])) {
//                 // Case 2.2: If the opposite child of the 'dir' child is red, do a double rotate
//                 node = doubleRotate(node, !dir);
//             }
//         }
//     }
//     return node;
// }
