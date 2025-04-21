
// https://www.happycoders.eu/algorithms/avl-tree-java/
// https://www.w3schools.com/dsa/dsa_data_avltrees.php
// https://www.happycoders.eu/algorithms/red-black-tree-java/?utm_source=chatgpt.com
// https://zarif98sjs.github.io/blog/blog/redblacktree/
// https://llego.dev/posts/how-to-implement-red-black-tree-python/
// https://www.youtube.com/watch?v=bWNyAU4BpSw
// https://www.youtube.com/watch?v=ZxDruXbksR4&list=PLxsP6a6cqUZjtci4kJePNkF53As7J_2_L&index=3
// https://www.youtube.com/watch?v=qvZGUFHWChY
// https://www.youtube.com/watch?v=5IBxA-bZZH8
// https://medium.com/basecs/painting-nodes-black-with-red-black-trees-60eacb2be9a5
// https://medium.com/basecs/the-little-avl-tree-that-could-86a3cae410c7

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../include/hybrid_tree_api.h"

#define IS_RED(n) ((n) != NULL && (n)->color == RED)

#define REBALANCE_THRESHOLD 10
#define CAPACiTY 1000

// Core Functions
HybridTree *create_hybrid_tree()
{
    HybridTree *tree = (HybridTree *)malloc(sizeof(HybridTree));
    if (!tree)
    {
        fprintf(stderr, "Memory allocation failed for HybridTree.\n");
        return NULL;
    }

    tree->root = NULL; // Initially the tree is empty (root is NULL)
    tree->size = 0;    // No nodes initially
    return tree;
}

void *int_to_void_ptr(int key)
{
    return (void *)(intptr_t)key;
}

int void_ptr_to_int(void *ptr)
{
    return (int)(intptr_t)ptr;
}

HybridNode *create_hybrid_node(int key, void *value)
{
    struct HybridNode *node = (struct HybridNode *)malloc(sizeof(struct HybridNode));

    if (!node)
    {
        fprintf(stderr, "Memory allocation failed for hybrid_node.\n");
        return NULL;
    }

    node->key = int_to_void_ptr(key); // Convert int to void pointer
    node->value = value;              // ✅ Store the passed-in value

    node->color = RED; // New nodes are RED
    node->height = 1;  // AVL height
    node->access_count = 0;

    node->parent = NULL;
    node->child[LEFT] = NULL;
    node->child[RIGHT] = NULL;

    return node;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int get_height(struct HybridNode *node)
{
    return node != NULL ? node->height : -1;
}

void update_height(struct HybridNode *node)
{
    int left_child_height = get_height(node->child[LEFT]);
    int right_child_height = get_height(node->child[RIGHT]);
    node->height = max(left_child_height, right_child_height) + 1;
}

int get_balance(struct HybridNode *node)
{
    if (node == NULL)
    {
        return 0;
    }

    return get_height(node->child[LEFT]) - get_height(node->child[RIGHT]);
}

HybridNode *avl_rotate_left(HybridNode *node)
{
    struct HybridNode *new_position = node->child[RIGHT];

    node->child[RIGHT] = new_position->child[LEFT];
    new_position->child[LEFT] = node;

    update_height(node);         // Update height of the rotated node
    update_height(new_position); // Update height of the new root

    return new_position; // Return the new root of the subtree
}

HybridNode *avl_rotate_right(HybridNode *node)
{
    struct HybridNode *new_position = node->child[LEFT];

    node->child[LEFT] = new_position->child[RIGHT];
    new_position->child[RIGHT] = node;

    update_height(node);         // Update height of the rotated node
    update_height(new_position); // Update height of the new root

    return new_position; // Return the new root of the subtree
}

HybridNode *rebalance_if_needed(HybridTree *tree, HybridNode *node)
{
    int balance = get_balance(node);

    // Left heavy case
    if (balance > 1)
    {
        // Left-Left case
        if (get_balance(node->child[LEFT]) >= 0)
        {
            node = avl_rotate_right(node);
        }
        // Left-Right case
        else
        {
            node->child[LEFT] = avl_rotate_left(node->child[LEFT]);
            node = avl_rotate_right(node);
        }
    }
    // Right heavy case
    else if (balance < -1)
    {
        // Right-Right case
        if (get_balance(node->child[RIGHT]) <= 0)
        {
            node = avl_rotate_left(node);
        }
        // Right-Left case
        else
        {
            node->child[RIGHT] = avl_rotate_right(node->child[RIGHT]);
            node = avl_rotate_left(node);
        }
    }

    // Update the height after rebalancing
    update_height(node);

    return node; // Return the potentially rebalanced node
}

void color_flip(HybridNode *node)
{
    if (!node)
        return; // Ensure node is not NULL

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

void swap_keys(HybridNode *a, HybridNode *b)
{
    int temp = void_ptr_to_int(a->key);
    a->key = b->key;
    b->key = int_to_void_ptr(temp);
}

HybridNode *rotate(struct HybridTree *tree, HybridNode *node, Direction dir)
{
    struct HybridNode *new_root = node->child[!dir]; // Get the child to rotate up

    if (!new_root)
        return node; // Safety check: return the original node if no child to rotate

    node->child[!dir] = new_root->child[dir]; // Move the subtree
    if (new_root->child[dir])
    {
        new_root->child[dir]->parent = node; // Update parent pointer
    }

    new_root->child[dir] = node;     // Perform the rotation
    new_root->parent = node->parent; // Preserve the original parent's pointer

    // Update the parent's child pointer if necessary
    if (node->parent)
    {
        if (node->parent->child[LEFT] == node)
            node->parent->child[LEFT] = new_root;
        else
            node->parent->child[RIGHT] = new_root;
    }
    else
    {
        tree->root = new_root; // Update the root if the node was the root
    }

    node->parent = new_root; // Update the original node's parent

    // Swap colors (needed for Red-Black Tree balancing)
    new_root->color = node->color;
    node->color = RED;

    // Update heights for AVL balancing
    update_height(node);
    update_height(new_root);

    return new_root; // Return the new root after the rotation
}

HybridNode *double_rotate(HybridTree *tree, HybridNode *node, Direction dir)
{
    // Perform the first rotation in the opposite direction
    node->child[!dir] = rotate(tree, node->child[!dir], (Direction)!dir);
    // Perform the second rotation in the given direction
    return rotate(tree, node, dir);
}

HybridNode *rb_insert_fixup(HybridTree *tree, HybridNode *node, Direction dir)
{
    while (node->parent && node->parent->color == RED)
    {
        struct HybridNode *grandparent = node->parent->parent;

        if (grandparent == NULL)
            break; // No grandparent, exit the loop

        struct HybridNode *uncle = grandparent->child[!dir];

        if (uncle && uncle->color == RED)
        {
            // Case 1: Uncle is red
            node->parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            node = grandparent; // Move up the tree
        }
        else
        {
            // Case 2: Uncle is black or NULL, and we need to rotate
            if (node == node->parent->child[!dir])
            {
                // Perform rotation to make the node the outer child
                node = node->parent;
                rotate(tree, node, dir);
            }

            // Perform rotation to make the grandparent the inner child
            node->parent->color = BLACK;
            grandparent->color = RED;
            rotate(tree, grandparent, (Direction)!dir);
        }
    }

    // Ensure the root is black
    tree->root->color = BLACK;

    return node;
}

// Function definition in implementation file (hybrid_tree_api.c)
HybridNode *insert_hybrid(HybridTree *tree, HybridNode *node, int key, void *value, Direction dir, bool *fixup_ok)
{
    if (node == NULL)
    {
        *fixup_ok = true;
        tree->size++;
        HybridNode *new_node = create_hybrid_node(key, value);
        return new_node;
    }

    int cmp = key - void_ptr_to_int(node->key);

    if (cmp < 0)
    {
        HybridNode *inserted = insert_hybrid(tree, node->child[LEFT], key, value, LEFT, fixup_ok);
        node->child[LEFT] = inserted;
        if (inserted)
            inserted->parent = node;
    }
    else if (cmp > 0)
    {
        HybridNode *inserted = insert_hybrid(tree, node->child[RIGHT], key, value, RIGHT, fixup_ok);
        node->child[RIGHT] = inserted;
        if (inserted)
            inserted->parent = node;
    }
    else
    {
        // Duplicate key — do not insert again
        *fixup_ok = false;
        return node;
    }

    // AVL balancing
    node = rebalance_if_needed(tree, node);

    // Red-Black fix-up
    node = rb_insert_fixup(tree, node, dir);

    return node;
}

void insert_hybrid_public(HybridTree *tree, int key, void *value, bool *inserted)
{
    if (!tree || !inserted)
        return;

    Direction dir = LEFT; // Just for initial fixup
    bool fixup_ok = false;

    tree->root = insert_hybrid(tree, tree->root, key, value, dir, &fixup_ok);

    *inserted = fixup_ok;

    if (tree->root)
        tree->root->color = BLACK;
}

HybridNode *rb_delete_fixup(HybridTree *tree, HybridNode *node, bool dir, bool *ok)
{
    struct HybridNode *parent = node;
    struct HybridNode *sibling = node->child[!dir];

    // Case 1: Red sibling — reduce to Black Sibling Case via rotation
    if (IS_RED(sibling))
    {
        node = rotate(tree, node, (Direction)dir);
        sibling = parent->child[!dir]; // re-fetch sibling after rotation
    }

    if (sibling != NULL)
    {
        // Case 2: Black sibling with black or null children
        if (!IS_RED(sibling->child[LEFT]) && !IS_RED(sibling->child[RIGHT]))
        {
            if (IS_RED(parent))
                *ok = true;

            parent->color = BLACK;
            sibling->color = RED;
        }
        else
        {
            Color original_color = parent->color;
            bool update_child = (node != parent); // if we're deeper in recursion

            if (IS_RED(sibling->child[!dir])) // Outer child is red (RR or LL)
            {
                parent = rotate(tree, parent, (Direction)dir);
            }
            else // Inner child is red (RL or LR)
            {
                parent = double_rotate(tree, parent, (Direction)dir);
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

HybridNode *find_minimum(HybridNode *node)
{
    while (node->child[0] != NULL)
    {
        node = node->child[0];
    }

    return node;
}

HybridNode *find_maximum(HybridNode *node)
{
    while (node->child[1] != NULL)
    {
        node = node->child[1];
    }

    return node;
}

HybridNode *delete_hybrid(HybridTree *tree, HybridNode *node, int key, Direction dir, bool *fixup_ok)
{
    if (node == NULL)
        return NULL;

    if (key < void_ptr_to_int(node->key))
    {
        dir = LEFT;
        node->child[LEFT] = delete_hybrid(tree, node->child[LEFT], key, dir, fixup_ok);
    }
    else if (key > void_ptr_to_int(node->key))
    {
        dir = RIGHT;
        node->child[RIGHT] = delete_hybrid(tree, node->child[RIGHT], key, dir, fixup_ok);
    }
    else
    {
        // Found the node to delete
        if (node->child[LEFT] == NULL || node->child[RIGHT] == NULL)
        {
            // One or no child
            struct HybridNode *temp = node->child[LEFT] ? node->child[LEFT] : node->child[RIGHT];

            if (!temp)
            {
                // No child case
                free(node);
                tree->size--;
                return NULL;
            }
            else
            {
                // One child case: replace node with its child
                struct HybridNode *to_free = node;
                node = temp;
                free(to_free);
                tree->size--;
                return node;
            }
        }
        else
        {
            // Node with two children: find inorder successor
            struct HybridNode *successor = find_minimum(node->child[RIGHT]);

            // Swap keys
            swap_keys(node, successor);

            // Delete successor (which now holds the key to delete)
            dir = RIGHT;
            node->child[RIGHT] = delete_hybrid(tree, node->child[RIGHT], key, dir, fixup_ok);
        }
    }

    // Rebalancing
    node = rebalance_if_needed(tree, node);
    node = rb_delete_fixup(tree, node, dir, fixup_ok);

    return node;
}

void delete_from_hybrid_tree(HybridTree *tree, int key)
{
    bool fixup_ok = false;
    tree->root = delete_hybrid(tree, tree->root, key, LEFT, &fixup_ok);
}

// hybrid_tree_api.h
HybridNode *search_hybrid(HybridTree *tree, void *key)
{
    if (!tree || !key)
        return NULL;

    HybridNode *node = tree->root;
    int search_key = void_ptr_to_int(key); // Convert void * to int

    while (node)
    {
        int node_key = void_ptr_to_int(node->key); // Also convert stored key
        if (search_key == node_key)
            return node;

        if (search_key < node_key)
            node = node->child[LEFT];
        else
            node = node->child[RIGHT];
    }

    return NULL;
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

    struct HybridNode *parent = node->parent;

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

    struct HybridNode *parent = node->parent;

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

        printf("Node with key %d has been accessed %d times.\n", void_ptr_to_int(node->key), node->access_count);
    }
}

void inorder_traversal(HybridNode *node)
{
    if (!node)
        return;

    inorder_traversal(node->child[LEFT]);
    printf("%d ", void_ptr_to_int(node->key));
    inorder_traversal(node->child[RIGHT]);
}

void print_hybrid_tree(HybridNode *root, int level)
{
    inorder_traversal(root);
    printf("\n");
}

void print_tree(HybridNode *root, int space)
{
    if (root == NULL)
        return;

    space += 5;

    print_tree(root->child[RIGHT], space);

    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%d (%s)\n", void_ptr_to_int(root->key), root->color == RED ? "R" : "B");

    print_tree(root->child[LEFT], space);
}

void destroy_hybrid_tree(HybridTree *tree)
{
    if (!tree)
        return;

    // Free all nodes in the tree
    free_hybrid_tree(tree->root, NULL); // or pass a free_key function if needed

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
        free_key(node->key);
    free(node);
}

HybridNode *tree_map_insert_hybrid(HashMapWithTree *map, HybridTree *tree, int key, void *value)
{
    if (!map)
        return NULL;

    unsigned int index = hash(key, map->capacity);
    bool was_inserted = false;

    insert_hybrid_public(map->buckets[index], key, value, &was_inserted);

    return search_hybrid(map->buckets[index], int_to_void_ptr(key));
}

// Deletes a TreeMap key from the HybridTree
void tree_map_delete_hybrid(HashMapWithTree *map, HybridTree *tree, int key)
{
    if (!tree || !tree->root)
        return;

    bool fixup_ok = false;
    Direction dir = LEFT; // Default direction (will be set properly in deletion)
    tree->root = delete_hybrid(tree, tree->root, key, dir, &fixup_ok);
}

// Searches for a key in the TreeMap via the HybridTree
HybridNode *tree_map_search_hybrid(HashMapWithTree *map, HybridTree *tree, int key)
{
    if (!tree || !tree->root)
        return NULL;

    struct HybridNode *current = tree->root;

    while (current)
    {
        if (key == void_ptr_to_int(current->key))
            return current;
        else if (key < void_ptr_to_int(current->key))
            current = current->child[LEFT];
        else
            current = current->child[RIGHT];
    }

    return NULL;
}

// Visit left -> self -> right
void range_query(HybridNode *node, int low, int high, DynamicArray *result)
{
    if (!node)
        return;

    // Convert node's key to integer (adjust if the key is of different type)
    int key = void_ptr_to_int(node->key);

    // Traverse left subtree if there's potential for keys in range
    if (key > low)
        range_query(node->child[0], low, high, result);

    // Include current node if within range
    if (key >= low && key <= high)
    {
        insert_into_dynamic_array(result, node); // Ensure this correctly adds the node to the result
    }

    // Traverse right subtree if there's potential for keys in range
    if (key < high)
        range_query(node->child[1], low, high, result);
}

void treemap_range_query(HybridTree *tree, int low, int high)
{
    if (!tree)
        return;

    DynamicArray *result = create_dynamic_array(100);
    if (!result)
    {
        fprintf(stderr, "Failed to allocate dynamic array for range query.\n");
        return;
    }

    range_query(tree->root, low, high, result);

    printf("Range query result: Keys in range [%d, %d]: ", low, high);
    for (int i = 0; i < result->size; i++)
    {
        HybridNode *node = (HybridNode *)result->items[i];
        printf("%d ", void_ptr_to_int(node->key));
    }
    printf("\n");

    free_dynamic_array(result);
}

// int main()
// {
//     // Create a new hybrid tree with initial capacity of 100
//     struct HybridTree *tree = create_hybrid_tree(100);
//     if (!tree)
//     {
//         fprintf(stderr, "Failed to create hybrid tree\n");
//         return 1;
//     }

//     // Insert some test values
//     printf("Inserting values into the tree...\n");
//     bool inserted = false;
//     insert_hybrid_public(tree, 5, int_to_void_ptr(5), &inserted);
//     insert_hybrid_public(tree, 2, int_to_void_ptr(2), &inserted);
//     insert_hybrid_public(tree, 8, int_to_void_ptr(8), &inserted);
//     insert_hybrid_public(tree, 3, int_to_void_ptr(3), &inserted);
//     insert_hybrid_public(tree, 9, int_to_void_ptr(9), &inserted);

//     // Print the tree structure
//     printf("\nTree structure:\n");
//     print_tree(tree->root, 0);

//     // Search for a value
//     printf("\nSearching for key 8...\n");
//     HybridNode *found = search_hybrid(tree, int_to_void_ptr(8));
//     if (found)
//     {
//         printf("Found node with key %d (color: %s)\n",
//                void_ptr_to_int(found->key),
//                found->color == RED ? "RED" : "BLACK");
//     }
//     else
//     {
//         printf("Key 8 not found.\n");
//     }

//     // Delete a value
//     printf("\nDeleting key 2...\n");
//     delete_from_hybrid_tree(tree, 2);

//     // Print updated tree
//     printf("\nTree after deletion:\n");
//     print_tree(tree->root, 0);

//     // Perform a range query
//     printf("\nRange query from 3 to 8:\n");
//     DynamicArray *result = create_dynamic_array(10);
//     treemap_range_query(tree, 3, 8);

//     for (int i = 0; i < result->size; ++i)
//     {
//         void *item = get_from_dynamic_array(result, i);
//         printf("%d ", void_ptr_to_int(item));
//     }
//     printf("\n");

//     // Clean up
//     // free_dynamic_array(result);
//     // destroy_hybrid_tree(tree);

//     return 0;
// }
