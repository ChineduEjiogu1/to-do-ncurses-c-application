// tree_map.c

#include <stdlib.h>
#include "tree_map_api.h"

TreeMap *create_tree_map() {
    TreeMap *map = malloc(sizeof(TreeMap));
    if (!map) return NULL;

    map->tree = create_hybrid_tree(1000);  // You'll need this function
    return map;
}

bool tree_map_insert(TreeMap *map, int key) {
    if (!map || !map->tree) return false;

    bool inserted = false;
    insert_hybrid_public(map->tree, key, &inserted);
    return inserted;
}

bool tree_map_delete(TreeMap *map, int key) {
    if (!map || !map->tree) return;

    delete_from_hybrid_tree(map->tree, key);
}

bool tree_map_search(TreeMap *map, int key) {
    if (!map || !map->tree) return false;

    return (search_hybrid(map->tree->root, key) != NULL);
}

void tree_map_print(TreeMap* map) 
{
    print_tree(map->tree->root, 0);
}

void tree_map_range_query(TreeMap *map, int low, int high, DynamicArray *result) {
    if (!map || !map->tree) return;

    printf("Keys in range [%d, %d]: ", low, high);
    range_query(map->tree->root, low, high, result);

    printf("\n");
}

void free_tree_map(TreeMap *map) {
    if (!map) return;

    if (map->tree) {
        free_hybrid_tree(map->tree->root, NULL); // or pass a free_key function if needed
        free(map->tree); // free the HybridTree struct itself
    }

    free(map);
}

int main() {
    TreeMap *map = create_tree_map();

    tree_map_insert(map, 5);
    tree_map_insert(map, 3);
    tree_map_insert(map, 9);
    tree_map_insert(map, 8);

    tree_map_print(map);

    DynamicArray *range = create_dynamic_array(10);
    tree_map_range_query(map, 4, 10, range);

    printf("Range query result:\n");
    for (int i = 0; i < range->size; i++) {
        printf("%d ", range->items[i]->key);
    }
    printf("\n");

    tree_map_delete(map, 3);
    printf("After deletion:\n");
    tree_map_print(map);

    free_dynamic_array(range);
    free_tree_map(map);
    return 0;
}
