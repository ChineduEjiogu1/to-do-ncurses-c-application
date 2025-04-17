
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

#include "../include/hybrid_tree_api.h"

#define IS_RED(n) ((n) != NULL && (n)->color == RED)

#define REBALANCE_THRESHOLD 10
#define CAPACiTY 1000

// Core Functions
struct HybridTree *create_hybrid_tree(int capacity)
{
    struct HybridTree *tree = (struct HybridTree *)malloc(sizeof(HybridTree));

    if (!tree)
    {
        fprintf(stderr, "Memory allocation failed for hybrid_tree.\n");
        return NULL;
    }

    tree->size = 0;
    tree->capacity = (capacity > 0) ? capacity : 10; // Default capacity = 10 if invalid
    tree->root = NULL;                               // Ensure tree starts empty

    return tree;
}

struct HybridNode *create_hybrid_node(int key)
{
    struct HybridNode *node = (struct HybridNode *)malloc(sizeof(struct HybridNode));

    if (!node)
    {
        fprintf(stderr, "Memory allocation failed for hybrid_node.\n");
        return NULL;
    }

    node->key = key;
    node->color = RED;      // New nodes are always RED in Red-Black Trees
    node->height = 1;       // AVL trees start with height 1
    node->access_count = 0; // No accesses yet

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

struct HybridNode *avl_rotate_left(struct HybridNode *node)
{
    struct HybridNode *new_position = node->child[RIGHT];

    node->child[RIGHT] = new_position->child[LEFT];
    new_position->child[LEFT] = node;

    update_height(node);         // Update height of the rotated node
    update_height(new_position); // Update height of the new root

    return new_position; // Return the new root of the subtree
}

struct HybridNode *avl_rotate_right(struct HybridNode *node)
{
    struct HybridNode *new_position = node->child[LEFT];

    node->child[LEFT] = new_position->child[RIGHT];
    new_position->child[RIGHT] = node;

    update_height(node);         // Update height of the rotated node
    update_height(new_position); // Update height of the new root

    return new_position; // Return the new root of the subtree
}

struct HybridNode *rebalance_if_needed(struct HybridTree *tree, struct HybridNode *node)
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

void color_flip(struct HybridNode *node)
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

void swap_keys(struct HybridNode *a, struct HybridNode *b)
{
    int temp = a->key;
    a->key = b->key;
    b->key = temp;
}

struct HybridNode *rotate(struct HybridTree *tree, struct HybridNode *node, Direction dir)
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

struct HybridNode *double_rotate(struct HybridTree *tree, struct HybridNode *node, Direction dir)
{
    // Perform the first rotation in the opposite direction
    node->child[!dir] = rotate(tree, node->child[!dir], (Direction)!dir);
    // Perform the second rotation in the given direction
    return rotate(tree, node, dir);
}

struct HybridNode *rb_insert_fixup(struct HybridTree *tree, struct HybridNode *node, Direction dir)
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
struct HybridNode *insert_hybrid(struct HybridTree *tree, struct HybridNode *node, int key, Direction dir, bool *fixup_ok)
{
    // Base case: When we find an empty spot (NULL node), insert the new node
    if (node == NULL)
    {
        struct HybridNode *new_node = create_hybrid_node(key);
        tree->size++;
        return new_node;
    }

    // Traverse the tree based on the key comparison
    if (key < node->key)
    {
        // Go to the left child
        node->child[LEFT] = insert_hybrid(tree, node->child[LEFT], key, LEFT, fixup_ok);
    }
    else if (key > node->key)
    {
        // Go to the right child
        node->child[RIGHT] = insert_hybrid(tree, node->child[RIGHT], key, RIGHT, fixup_ok);
    }
    else
    {
        // Duplicate key found, return without modifying the tree
        return node;
    }

    // After insertion, first apply AVL balancing
    node = rebalance_if_needed(tree, node);

    // Then apply Red-Black tree balancing (fixup)
    node = rb_insert_fixup(tree, node, dir);

    return node;
}

void insert_hybrid_public(struct HybridTree *tree, int key, bool *inserted)
{
    if (tree == NULL || inserted == NULL)
        return;

    Direction dir = LEFT; // direction placeholder
    bool fixup_ok = false;

    // Insert the key into the tree and set the root as necessary
    tree->root = insert_hybrid(tree, tree->root, key, dir, &fixup_ok);

    // Set the inserted flag based on whether the key was successfully inserted
    *inserted = fixup_ok;

    // Ensure the root is always black
    if (tree->root)
        tree->root->color = BLACK;
}

struct HybridNode *rb_delete_fixup(struct HybridTree *tree, struct HybridNode *node, bool dir, bool *ok)
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

struct HybridNode *find_minimum(struct HybridNode *node)
{
    while (node->child[0] != NULL)
    {
        node = node->child[0];
    }

    return node;
}

struct HybridNode *find_maximum(struct HybridNode *node)
{
    while (node->child[1] != NULL)
    {
        node = node->child[1];
    }

    return node;
}

struct HybridNode *delete_hybrid(struct HybridTree *tree, struct HybridNode *node, int key, Direction dir, bool *fixup_ok)
{
    if (node == NULL)
        return NULL;

    if (key < node->key)
    {
        dir = LEFT;
        node->child[LEFT] = delete_hybrid(tree, node->child[LEFT], key, dir, fixup_ok);
    }
    else if (key > node->key)
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

void delete_from_hybrid_tree(struct HybridTree *tree, int key)
{
    bool fixup_ok = false;
    tree->root = delete_hybrid(tree, tree->root, key, LEFT, &fixup_ok);
}

struct HybridNode *search_hybrid(struct HybridTree *tree, int key)
{
    if (!tree || !tree->root)
        return NULL; // Tree is empty, key not found

    struct HybridNode *node = tree->root; // Start from the root

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

bool hybrid_tree_is_full(struct HybridTree *tree)
{
    return tree->size >= tree->capacity;
}

struct HybridNode *find_predecessor(struct HybridNode *node)
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

struct HybridNode *find_successor(struct HybridNode *node)
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

void increment_access_count(struct HybridNode *node)
{
    if (node != NULL)
    {
        node->access_count++;

        printf("Node with key %d has been accessed %d times.\n", node->key, node->access_count);
    }
}

void inorder_traversal(struct HybridNode *node)
{
    if (!node)
        return;

    inorder_traversal(node->child[LEFT]);
    printf("%d ", node->key);
    inorder_traversal(node->child[RIGHT]);
}

void print_hybrid_tree(struct HybridNode *root, int level)
{
    inorder_traversal(root);
    printf("\n");
}

void print_tree(struct HybridNode *root, int space)
{
    if (root == NULL)
        return;

    space += 5;

    print_tree(root->child[RIGHT], space);

    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%d (%s)\n", root->key, root->color == RED ? "R" : "B");

    print_tree(root->child[LEFT], space);
}

void destroy_hybrid_tree(struct HybridTree *tree)
{
    if (!tree)
        return;

    // Free all nodes in the tree
    free_hybrid_tree(tree->root, NULL); // or pass a free_key function if needed

    // Finally, free the tree struct itself
    free(tree);
}

void free_hybrid_tree(struct HybridNode *node, void (*free_key)(void *))
{
    if (!node)
        return;

    free_hybrid_tree(node->child[LEFT], free_key);
    free_hybrid_tree(node->child[RIGHT], free_key);

    if (free_key)
        free_key((void *)&node->key);

    free(node);
}

struct HybridNode *tree_map_insert_hybrid(struct HashMapWithTree *map, struct HybridTree *tree, int key) {
    if (!map) return NULL;

    unsigned int index = hash(key, map->capacity);
    bool was_inserted = false;

    insert_hybrid_public(map->buckets[index], key, &was_inserted);

    // Return the inserted node by searching for it
    return search_hybrid(map->buckets[index], key);
}

// Deletes a TreeMap key from the HybridTree
void tree_map_delete_hybrid(TreeMap *map, struct HybridTree *tree, int key)
{
    if (!tree || !tree->root)
        return;

    bool fixup_ok = false;
    Direction dir = LEFT; // Default direction (will be set properly in deletion)
    tree->root = delete_hybrid(tree, tree->root, key, dir, &fixup_ok);
}

// Searches for a key in the TreeMap via the HybridTree
struct HybridNode *tree_map_search_hybrid(TreeMap *map, struct HybridTree *tree, int key)
{
    if (!tree || !tree->root)
        return NULL;

    struct HybridNode *current = tree->root;

    while (current)
    {
        if (key == current->key)
            return current;
        else if (key < current->key)
            current = current->child[LEFT];
        else
            current = current->child[RIGHT];
    }

    return NULL;
}

// Visit left -> self -> right
void range_query(struct HybridNode *node, int low, int high, struct DynamicArray *result)
{
    if (!node)
    {
        return; // Early exit if node is null (empty tree or reached a leaf)
    }

    // Call in-order range query recursively
    in_order_range_query(node, low, high, result);
}

void in_order_range_query(struct HybridNode *node, int low, int high, struct DynamicArray *result)
{
    if (!node)
        return;

    // Traverse left subtree
    in_order_range_query(node->child[0], low, high, result);

    // Check if the current node's key is within the range
    if (node->key >= low && node->key <= high)
    {
        insert_into_dynamic_array(result, node); // Insert the node, not just the key
    }

    // Traverse right subtree
    in_order_range_query(node->child[1], low, high, result);
}

void treemap_range_query(struct HybridTree *tree, int low, int high)
{
    struct DynamicArray *result = create_dynamic_array(100); // initial capacity
    range_query(tree->root, low, high, result);

    printf("Range query result: Keys in range [%d, %d]: ", low, high);
    for (int i = 0; i < result->size; i++)
    {
        struct HybridNode *node = (struct HybridNode *)result->items[i];
        printf("%d ", node->key);
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
//     insert_hybrid_public(tree, 5, &inserted);
//     insert_hybrid_public(tree, 2, &inserted);
//     insert_hybrid_public(tree, 8, &inserted);
//     insert_hybrid_public(tree, 3, &inserted);
//     insert_hybrid_public(tree, 9, &inserted);

//     // Print the tree structure
//     printf("\nTree structure:\n");
//     print_tree(tree->root, 0);

//     // Search for a value
//     printf("\nSearching for key 8...\n");
//     struct HybridNode *found = search_hybrid(tree, 8);
//     if (found)
//     {
//         printf("Found node with key %d (color: %s)\n",
//                found->key,
//                found->color == RED ? "RED" : "BLACK");
//     }

//     // Delete a value
//     printf("\nDeleting key 2...\n");
//     delete_from_hybrid_tree(tree, 2);

//     // Print updated tree
//     printf("\nTree after deletion:\n");
//     print_tree(tree->root, 0);

//     DynamicArray *result = create_dynamic_array(100);

//     treemap_range_query(tree, 3, 8);

//     printf("\n");

//     free_dynamic_array(result);

//     // Clean up
//     destroy_hybrid_tree(tree);
//     return 0;
// }