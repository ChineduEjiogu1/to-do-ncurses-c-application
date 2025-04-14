// tree_map.h

#ifndef TREE_MAP_H
#define TREE_MAP_H

#include "hybrid_tree_api.h"

typedef struct TreeMap {
   struct HybridTree *tree;
} TreeMap;

TreeMap* create_tree_map();
void destroy_tree_map(TreeMap* map);

void tree_map_insert(TreeMap* map, int key, void* value);
void* tree_map_get(TreeMap* map, int key);
void tree_map_remove(TreeMap* map, int key);
bool tree_map_contains(TreeMap* map, int key);

void tree_map_print(TreeMap* map); // for debugging

#endif
