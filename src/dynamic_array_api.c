#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "../include/dynamic_array_api.h"

DynamicArray *create_dynamic_array(int capacity)
{
    if (capacity <= 0)
    {
        capacity = 4; // Fallback to default minimum capacity
    }

    DynamicArray *new_array = (DynamicArray *)malloc(sizeof(DynamicArray));
    if (!new_array)
    {
        printf("Error: Memory allocation failed for DynamicArray struct.\n");
        return NULL;
    }

    new_array->items = (struct HybridNode **)calloc(capacity, sizeof(struct HybridNode *));
    if (!new_array->items)
    {
        printf("Error: Memory allocation failed for DynamicArray items.\n");
        free(new_array); // Clean up struct if items allocation fails
        return NULL;
    }

    new_array->size = 0;
    new_array->capacity = capacity;

    return new_array;
}

// Core Functions
bool insert_into_dynamic_array(DynamicArray *array, HybridNode *node)
{
    if (!array || !node)
    {
        printf("Error: NULL input to insert_into_dynamic_array\n");
        return false;
    }

    if (array->capacity == 0 && !resize_dynamic_array(array, 4))
    {
        printf("Error: Failed to initialize array capacity\n");
        return false;
    }

    if (array->size >= array->capacity && !resize_dynamic_array(array, array->capacity * 2))
    {
        printf("Error: Failed to resize dynamic array\n");
        return false;
    }

    array->items[array->size++] = node;
    return true;
}

bool remove_from_dynamic_array(DynamicArray *array, int index)
{
    if (!array || array->size == 0)
    {
        printf("Error: Cannot remove from an empty array.\n");
        return false;
    }

    if (index < 0 || index >= array->size)
    {
        printf("Error: Index %d is out of bounds (size: %d).\n", index, array->size);
        return false;
    }

    // Optional: Clear the element at index (especially if it owns heap memory)
    array->items[index] = NULL;

    // Shift elements only if not removing the last element
    for (int i = index; i < array->size - 1; i++)
    {
        array->items[i] = array->items[i + 1];
    }

    array->size--;

    // Optionally shrink the capacity, but not too small
    if (array->capacity > 4 && array->size < array->capacity / 4)
    {
        resize_dynamic_array(array, array->capacity / 2);
    }

    return true;
}

struct HybridNode *get_from_dynamic_array(DynamicArray *array, int index)
{
    if (!array || index < 0 || index >= array->size)
    {
        return NULL; // Index out of bounds
    }
    return array->items[index]; // Return the Node* at the given index
}

void free_dynamic_array(DynamicArray *array)
{
    if (!array)
        return; // Prevent freeing NULL pointer

    // Free stored Nodes if they were dynamically allocated
    for (int i = 0; i < array->size; i++)
    {
        free(array->items[i]); // Free each Node (if applicable)
    }

    free(array->items); // Free the array of pointers
    free(array);        // Free the struct itself
}

void clear_dynamic_array(DynamicArray *array)
{
    if (!array)
        return; // Prevent NULL pointer access

    // Free each dynamically allocated Node* (if needed)
    for (int i = 0; i < array->size; i++)
    {
        free(array->items[i]); // Free each stored Node
    }

    array->size = 0; // Reset size but keep capacity the same
}

// Utility Functions
bool resize_dynamic_array(DynamicArray *array, int new_capacity)
{
    if (!array || new_capacity <= array->size)
        return false;

    struct HybridNode **new_data = (struct HybridNode **)malloc(new_capacity * sizeof(struct HybridNode *));
    if (!new_data)
        return false;

    for (int i = 0; i < array->size; i++)
        new_data[i] = array->items[i];

    free(array->items);
    array->items = new_data;
    array->capacity = new_capacity;

    return true; // <- Add this
}

bool is_dynamic_array_empty(DynamicArray *array)
{
    return array->size == 0;
}

int dynamic_array_size(DynamicArray *array)
{
    return array->size;
}