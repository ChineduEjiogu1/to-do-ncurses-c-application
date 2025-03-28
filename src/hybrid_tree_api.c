#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/hybrid_tree_api.h"
#include "../include/dynamic_array_api.h"

#define REBALANCE_THRESHOLD 10

// Core Functions
HybridTree *create_hybrid_tree(int capacity)
{
    HybridTree *tree = (HybridTree *)malloc(sizeof(HybridTree));

    if (!tree)
    {
        printf("Memory allocation failed for HybridTree.\n");
        return NULL;
    }

    tree->size = 0;
    tree->capacity = capacity > 0 ? capacity : 10; // Default to 10 if capacity is invalid
    tree->root = NULL;                             // Make sure the root starts off empty

    return tree;
}

HybridNode *create_hybrid_node(int key)
{
    HybridNode *node = (HybridNode *)malloc(sizeof(HybridNode));

    if (!node)
    {
        printf("Memory allocation failed for HybridNode.\n");
        return NULL;
    }

    node->key = key;
    node->color = RED;      // New nodes are typically RED in Red-Black Trees
    node->height = 1;       // Initial height for AVL balancing
    node->access_count = 0; // Start access count at 0 for now

    node->parent = NULL;
    node->child[0] = node->child[1] = NULL; // Initialize both child pointers to NULL
    node->left = node->right = NULL;        // If you're using left/right too, ensure these are NULL

    return node;
}

HybridNode *rotate(HybridTree *tree, HybridNode *node, int dir)
{
    HybridNode *temp = node->child[!dir];

    // Rearrange pointers
    node->child[!dir] = temp->child[dir];
    if (temp->child[dir])
        temp->child[dir]->parent = node;

    temp->child[dir] = node;
    temp->parent = node->parent;
    node->parent = temp;

    // If node was the root, update tree root
    if (temp->parent == NULL)
        tree->root = temp;
    else
    {
        // Connect temp to its new parent
        if (node == temp->parent->child[0])
            temp->parent->child[0] = temp;
        else
            temp->parent->child[1] = temp;
    }

    // Fix colors after rotation
    temp->color = node->color;
    node->color = RED;

    return temp;
}

void rebalance_avl(HybridTree *tree, HybridNode *node)
{
    while (node != NULL)
    {
        update_height(node);
        int balance = get_balance(node);

        // Left Heavy (LL or LR)
        if (balance > 1)
        {
            if (get_balance(node->left) < 0)
            {
                node->left = avl_rotate_left(node->left);
            }
            node = avl_rotate_right(node);
        }

        // Right Heavy (RR or RL)
        else if (balance < -1)
        {
            if (get_balance(node->right) > 0)
            {
                node->right = avl_rotate_right(node->right);
            }
            node = avl_rotate_left(node);
        }

        if (node->parent == NULL)
            tree->root = node; // Keep the root updated
        node = node->parent;
    }
}

// Helper function to count black nodes along a path
int count_black_nodes(HybridNode *node) {
    if (node == NULL) return 1; // NULL nodes are black
    int left_black_height = count_black_nodes(node->left);
    int right_black_height = count_black_nodes(node->right);

    // If any subtree is invalid, return -1
    if (left_black_height == -1 || right_black_height == -1 || left_black_height != right_black_height)
        return -1;

    // Add 1 if this node is black
    return left_black_height + (node->color == BLACK ? 1 : 0);
}

// Function to verify Red-Black tree properties
int is_red_black_tree(HybridNode *node, int is_root) {
    if (node == NULL) return 1; // An empty tree is a valid Red-Black tree

    // Property 1: The root must always be black
    if (is_root && node->color != BLACK) {
        printf("Violation: Root is not black.\n");
        return 0;
    }

    // Property 2: No red node can have a red child (no two consecutive reds)
    if (node->color == RED) {
        if ((node->left && node->left->color == RED) || (node->right && node->right->color == RED)) {
            printf("Violation: Red node has a red child.\n");
            return 0;
        }
    }

    // Recursively check left and right subtrees
    if (!is_red_black_tree(node->left, 0) || !is_red_black_tree(node->right, 0))
        return 0;

    // Property 3: Every path from root to leaf must have the same number of black nodes
    if (count_black_nodes(node) == -1) {
        printf("Violation: Black height property is not maintained.\n");
        return 0;
    }

    return 1;
}

// Wrapper function
int validate_red_black_tree(HybridTree *tree) {
    return is_red_black_tree(tree->root, 1);
}

// Insertion and Deletion
void insert_hybrid(HybridTree *tree, int key)
{
    if (!tree)
        return;

    HybridNode *new_node = create_hybrid_node(key);
    if (!new_node)
        return;

    // Standard BST insertion
    if (tree->root == NULL)
    {
        new_node->color = BLACK; // Root must be black in Red-Black Trees
        tree->root = new_node;
    }
    else
    {
        HybridNode *current = tree->root;
        HybridNode *parent = NULL;

        while (current != NULL)
        {
            parent = current;

            if (key < current->key)
                current = current->left;
            else if (key > current->key)
                current = current->right;
            else
                return; // No duplicate keys allowed
        }

        // Link the new node
        new_node->parent = parent;
        if (key < parent->key)
            parent->left = new_node;
        else
            parent->right = new_node;

        // **Rebalance using AVL height tracking**
        rebalance_avl(tree, new_node);

        // **Fix Red-Black properties**
        fix_red_black(tree, new_node);
    }

    tree->size++;
}

void delete_hybrid(HybridTree *tree, int key, int dir)
{
    HybridNode *node = search_hybrid(tree, key);

    if (node == NULL)
    {
        return; // Key not found, exit
    }

    int index = get_from_dynamic_array(node, key);

    if (index == -1) // Assuming -1 means key not found
    {
        return;
    }

    remove_from_dynamic_array(node, index);

    // If the dynamic array is empty, remove the node from the tree
    if (is_empty_dynamic_array(node))
    {
        HybridNode *successor = find_successor(node); // Needed for fixing tree balance
        delete_node(tree, node);

        if (is_red_black_tree(tree, 1)) // Check if tree is Red-Black
        {
            rb_delete_fixup(tree, successor, dir);
        }
        else
        {
            rebalance_avl(tree, successor);
        }
    }
}

// Searching and Access Count Management
HybridNode *search_hybrid(HybridTree *tree, int key)
{
    if (tree == NULL || tree->root == NULL)
        return NULL;

    HybridNode *current = tree->root;

    while (current != NULL)
    {
        if (key == current->key)  // Found the node
            return current;
        else if (key < current->key)
            current = current->left;
        else
            current = current->right;
    }

    return NULL;  // Key not found
}

void increment_access_count(HybridNode *node, HybridTree *tree) {
    if (node == NULL)
        return;

    node->access_count++;

    // If a node has been accessed too frequently, consider rebalancing
    if (node->access_count >= REBALANCE_THRESHOLD) {
        if (is_red_black_tree(tree, 1)) {
            rebalance_red_black(tree, node);
        } else {
            rebalance_avl(tree, node);
        }
        node->access_count = 0; // Reset count after rebalancing
    }
}

void color_flip(HybridNode *node)
{
    node->color = !node->color;
    if (node->left)
        node->left->color = !node->left->color;
    if (node->right)
        node->right->color = !node->right->color;
}

void fix_red_black(HybridTree *tree, HybridNode *node, int dir)
{
    while (node != tree->root && node->parent && node->parent->color == RED)
    {
        if (node->parent->parent == NULL) break; // Ensure grandparent exists
        HybridNode *grandparent = node->parent->parent;

        if (node->parent == grandparent->left)
        { // Parent is a left child
            HybridNode *uncle = grandparent->right;

            // Case 1: Uncle is red (recolor)
            if (uncle && uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                // Case 2: Node is right child (Left-Right case)
                if (node == node->parent->right)
                {
                    node = node->parent;
                    tree->root = rotate(tree, node, dir);
                }

                // Case 3: Node is left child (Left-Left case)
                node->parent->color = BLACK;
                grandparent->color = RED;
                tree->root = rotate(grandparent);
            }
        }
        else
        { // Parent is a right child
            HybridNode *uncle = grandparent->left;

            // Mirror of the first case
            if (uncle && uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                node = grandparent;
            }
            else
            {
                // Right-Left case
                if (node == node->parent->left)
                {
                    node = node->parent;
                    tree->root = rotate(node, );
                }

                // Right-Right case
                node->parent->color = BLACK;
                grandparent->color = RED;
                tree->root = rotate(tree, grandparent, dir);
            }
        }
    }

    tree->root->color = BLACK; // Root must always be black
}

// Red-Black Balancing Functions
HybridNode *rb_insert_fixup(HybridTree *tree, HybridNode *node, int dir)
{
    if (node == NULL || node->child[dir] == NULL)
        return node;

    // Case 1: Both children are red — flip colors
    if (is_red_black(node->child[0]) && is_red_black(node->child[1]))
    {
        node->color = RED;
        node->child[0]->color = BLACK;
        node->child[1]->color = BLACK;
    }
    else
    {
        // Case 2: One child has two reds in a row (LL or RR) — rotate
        if (node->child[dir] && node->child[dir]->child[dir] && is_red_black_tree(node->child[dir], tree) && is_red_black_tree(node->child[dir]->child[dir], tree))
        {
            node = rotate(tree, node, !dir);
        }
        // Case 3: Child has red grandchild in opposite direction (LR or RL) — double rotate
        else if (node->child[dir] && node->child[dir]->child[!dir] && is_red_black_tree(node->child[dir], tree) && is_red_black_tree(node->child[dir]->child[!dir], tree))
        {
            node = double_rotate(tree, node, !dir);
        }
    }

    tree->root->color = BLACK;

    return node;
}

// RB delete fixup function (improved)
HybridNode *rb_delete_fixup(HybridTree *tree, HybridNode *node, int dir)
{
    HybridNode *parent = node;
    HybridNode *sibling = node ? node->child[!dir] : NULL;

    if (sibling == NULL)
    {
        node = node->parent;
        return node;
    }

    if (is_red(sibling))
    {
        node = rotate(tree, node, dir);
        sibling = parent->child[!dir];
    }

    if (sibling)
    {
        if (!is_red_black_tree(sibling->child[LEFT]) && !is_red(sibling->child[RIGHT]))
        {
            parent->color = BLACK;
            sibling->color = RED;
        }
        else
        {
            int init_parent_color = parent->color;
            int isRedSiblingReduction = (node != parent);

            if (is_red(sibling->child[!dir]))
            {
                parent = rotate(tree, parent, dir);
            }
            else
            {
                parent = double_rotate(tree, parent, dir);
            }

            parent->color = init_parent_color;
            parent->child[LEFT]->color = BLACK;
            parent->child[RIGHT]->color = BLACK;

            if (isRedSiblingReduction)
            {
                node->child[dir] = parent;
            }
            else
            {
                node = parent;
            }
        }
    }

    tree->root->color = BLACK;
    return node;
}


HybridNode *double_rotate(HybridTree *tree, HybridNode *node, int dir)
{
    node->child[!dir] = rotate(tree, node->child[!dir], !dir);
    return rotate(tree, node, dir);
}

// AVL Balancing Functions
void update_height(HybridNode *node)
{
    if (node)
    {
        int left_height = (node->left) ? node->left->height : 0;
        int right_height = (node->right) ? node->right->height : 0;
        node->height = (left_height > right_height ? left_height : right_height) + 1;
    }
}

int get_balance(HybridNode *node)
{
    if (!node)
        return 0;
    int left_height = (node->left) ? node->left->height : 0;
    int right_height = (node->right) ? node->right->height : 0;
    return left_height - right_height;
}

HybridNode *avl_rotate_left(HybridNode *node)
{
    HybridNode *new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;

    new_root->parent = node->parent;
    node->parent = new_root;

    update_height(node);
    update_height(new_root);

    return new_root;
}

HybridNode *avl_rotate_right(HybridNode *node)
{
    HybridNode *new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;

    new_root->parent = node->parent;
    node->parent = new_root;

    update_height(node);
    update_height(new_root);

    return new_root;
}

void rebalance_avl(HybridTree *tree, HybridNode *node)
{
    while (node != NULL)
    {
        update_height(node);
        int balance = get_balance(node);

        // Left Heavy (LL or LR)
        if (balance > 1)
        {
            if (get_balance(node->left) < 0)
            {
                node->left = avl_rotate_left(node->left);
            }
            node = avl_rotate_right(node);
        }

        // Right Heavy (RR or RL)
        else if (balance < -1)
        {
            if (get_balance(node->right) > 0)
            {
                node->right = avl_rotate_right(node->right);
            }
            node = avl_rotate_left(node);
        }

        if (node->parent == NULL)
            tree->root = node; // Keep the root updated
        node = node->parent;
    }
}

// Utility Functions
void free_hybrid_tree(HybridNode *root)
{
    if (root == NULL)
        return;

    // Recursively free left and right subtrees
    free_hybrid_tree(root->left);
    free_hybrid_tree(root->right);

    // Free the dynamic array inside the node (if applicable)
    free_dynamic_array(root);

    // Free the node itself
    free(root);
}

void print_hybrid_tree(HybridNode *root, int level)
{
    if (root == NULL)
        return;

    // Increase indentation for better visualization
    level += 5;

    // Print right subtree first (to visualize better)
    print_hybrid_tree(root->right, level);

    // Print current node with indentation
    for (int i = 5; i < level; i++)
        printf(" ");

    printf("%d (", root->key);

    if (is_red_black_tree(root, 1))
        printf("RB");
    else
        printf("AVL");

    printf(")\n");

    // Print left subtree
    print_hybrid_tree(root->left, level);
}

// Successor and Predecessor
HybridNode *find_successor(HybridNode *node)
{
    if (!node)
    {
        return NULL;
    }

    if (node->right != NULL)
    {
        return find_minimum(node->right);
    }

    HybridNode *successor = node->parent;

    while (successor != NULL && node == successor->right)
    {
        node = successor;
        successor = successor->parent;
    }

    return successor;
}

HybridNode *find_predecessor(HybridNode *node)
{
    if (!node)
    {
        return NULL;
    }

    if (node->left != NULL)
    {
        return find_maximum(node->left); // Go to the max of the left subtree
    }

    HybridNode *predecessor = node->parent;

    while (predecessor != NULL && node == predecessor->left)
    {
        node = predecessor;
        predecessor = predecessor->parent; // Move up until node is a right child
    }

    return predecessor;
}

// Minimum and Maximum Key Functions
HybridNode *find_minimum(HybridNode *node)
{
    if (!node)
    {
        return NULL;
    }

    while (node->left != NULL)
    {
        node = node->left;
    }

    return node;
}

HybridNode *find_maximum(HybridNode *node)
{
    if (!node)
    {
        return NULL;
    }

    while (node->right != NULL)
    {
        node = node->right;
    }

    return node;
}

// Capacity Handling
bool hybrid_tree_is_full(HybridTree *tree)
{
    return tree->size >= tree->capacity;
}

// --- Integration Points with TreeMap --- //

// Inserts a TreeMap key into the HybridTree for faster access
void tree_map_insert_hybrid(TreeMap *map, HybridTree *tree, int key)
{
}

// Deletes a TreeMap key from the HybridTree
void tree_map_delete_hybrid(TreeMap *map, HybridTree *tree, int key)
{
}

// Searches for a key in the TreeMap via the HybridTree
HybridNode *tree_map_search_hybrid(TreeMap *map, HybridTree *tree, int key)
{
}

int main() 
{
    // Step 1: Create the HybridTree
    HybridTree *tree = create_hybrid_tree(100);

    // Step 2: Insert some values
    insert_hybrid(tree, 50);
    insert_hybrid(tree, 30);
    insert_hybrid(tree, 70);
    insert_hybrid(tree, 20);
    insert_hybrid(tree, 40);
    insert_hybrid(tree, 60);
    insert_hybrid(tree, 80);

    printf("HybridTree after insertions:\n");
    print_hybrid_tree(tree->root, 0);

    // Step 3: Search for a key
    int key_to_search = 40;
    HybridNode *found = search_hybrid(tree, key_to_search);
    if (found != NULL) {
        printf("\nKey %d found in the tree.\n", key_to_search);
    } else {
        printf("\nKey %d not found in the tree.\n", key_to_search);
    }

    // Step 4: Delete a node
    int key_to_delete = 30;
    delete_hybrid(tree, key_to_delete, 0); // Assuming 0 means left-child preference

    printf("\nHybridTree after deleting %d:\n", key_to_delete);
    print_hybrid_tree(tree->root, 0);

    // Step 5: Free memory
    free_hybrid_tree(tree->root);
    free(tree); // Free the HybridTree struct itself

    return 0;
}
