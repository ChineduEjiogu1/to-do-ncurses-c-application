#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "doubly_linked_list.h"
#include <stdbool.h>

typedef struct DynamicArray 
{
    Node **items;  // Array of pointers to linked list nodes
    int capacity;  // Total allocated capacity
    int size;      // Current number of elements
} DynamicArray;

// Core Functions
DynamicArray *create_dynamic_array(int capacity);
bool insert_into_dynamic_array(DynamicArray *array, Node *node);
bool remove_from_dynamic_array(DynamicArray *array, int index);
Node *get_from_dynamic_array(DynamicArray *array, int index);
void free_dynamic_array(DynamicArray *array);
void clear_dynamic_array(DynamicArray *array);

// Utility Functions
bool resize_dynamic_array(DynamicArray *array, int new_capacity);
bool is_dynamic_array_empty(DynamicArray *array);
int dynamic_array_size(DynamicArray *array);

#endif
