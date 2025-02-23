#include "tree_map_api.h"
#include <stdlib.h>
#include <stdbool.h>

#define LOAD_FACTOR_THRESHOLD 2.0

TreeMap *create_tree_map(int capacity)
{
    TreeMap *tree_map = (TreeMap *)malloc(sizeof(TreeMap));

    if (!tree_map)
    {
        printf("Memory allocation failed for initializing tree map. \n");
        return NULL;
    }

    tree_map->capacity = capacity;
    tree_map->load_factor = LOAD_FACTOR_THRESHOLD;
    tree_map->size = 0;

    tree_map->buckets = (AVLNode **)calloc(tree_map->capacity, sizeof(AVLNode *));

    if (!tree_map->buckets)
    {
        printf("Memory allocation failed for initializing buckets. \n");
        free(tree_map);
        return NULL;
    }

    return tree_map;
}

AVLNode *create_avl_node(int key, Node *node)
{
    AVLNode *avl_node = (AVLNode *)malloc(sizeof(AVLNode));

    if (!avl_node)
    {
        printf("Memory allocation failed for initializing AVL node.\n");
        return NULL;
    }

    avl_node->key = key;
    avl_node->height = 1;
    avl_node->left = avl_node->right = NULL;
    avl_node->node = node;

    return avl_node;
}

int hash_function(int capacity, int key)
{
    unsigned int hash = (unsigned int)key;
    hash = (hash * 2654435761) % capacity; // Knuth's Multiplicative Method
    return (int)hash;
}

bool needs_resizing(TreeMap *map)
{
    if((float)map->size / map->capacity >= LOAD_FACTOR_THRESHOLD)
    {
        return true;
    } 
    else
    {
        return false;
    }
}

TreeMapStatus resize_tree_map(TreeMap *map)
{

}

TreeMapStatus insert_into_tree_map(TreeMap *map, int key, Node *node)
{

}

TreeMapStatus delete_from_tree_map(TreeMap *map, int key)
{

}

TreeMapStatus search_in_tree_map(TreeMap *map, int key, Node **result)
{

}

TreeMapStatus insert_into_hybrid_from_treemap(TreeMap *map, int key, Node *node)
{

}

TreeMapStatus delete_from_hybrid_via_treemap(TreeMap *map, int key)
{

}

TreeMapStatus search_in_hybrid_from_treemap(TreeMap *map, int key, Node **result)
{

}

AVLNode *insert_into_avl(AVLNode *root, int key, Node *node)
{

}

AVLNode *delete_from_avl(AVLNode *root, int key)
{

}

AVLNode *find_avl_node(AVLNode *root, int key)
{

}

int tree_map_size(TreeMap *map)
{
    if (!map)
    {
        printf("Tree map is NULL.\n");
        return -1; // Indicates an error
    }

    return map->size;
}

bool tree_map_is_empty(TreeMap *map)
{
    if (!map)
    {
        printf("Tree map is NULL.\n");
        return true; // Consider an invalid map as empty
    }

    return map->size == 0;
}

void print_tree_map_table(TreeMap *map)
{

}

void free_avl_tree(AVLNode *root)
{

}

void free_tree_map(TreeMap *map)
{

}