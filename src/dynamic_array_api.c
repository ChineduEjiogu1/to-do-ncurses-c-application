#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "include/dynamic_array_api.h"

// Core Functions
DynamicArray *create_dynamic_array(int capacity)
{
    if (capacity <= 0)
    {
        return NULL;
    }

    DynamicArray *new_array = (DynamicArray *)malloc(sizeof(DynamicArray));

    if (!new_array)
    {
        printf("Memory allocation failed for Dynamic array.\n");
        return NULL;
    }

    if (!new_array->items)
    {
        printf("Memory allocation failed for array items.\n");
        return NULL;
    }

    for (int i = 0; i < capacity; i++)
    {
        new_array->items[i] = capacity;
    }

    new_array->capacity = capacity;
    new_array->size = 0;

    return new_array;
}

bool insert_into_dynamic_array(DynamicArray *array, Node *node)
{
    if (!array || !node)
    {
        return false; // Handle invalid input
    }

    // Resize if necessary
    if (array->size == array->capacity)
    {
        if (!resize_dynamic_array(array, array->capacity * 2))
        {
            return false; // Resize failed
        }
    }

    // Insert the new node
    array->items[array->size] = node;
    array->size++;

    return true;
}

bool remove_from_dynamic_array(DynamicArray *array, int index)
{
    if (!array || array->size == 0)
    {
        printf("Error: Array is empty\n");
        return false;
    }

    if (index < 0 || index >= array->size)
    {
        printf("Error: Index %d out of bounds\n", index);
        return false;
    }

    // Free the element at the given index (if needed)
    array->items[index] = NULL;

    // Shift elements to fill the gap
    for (int i = index; i < array->size - 1; i++)
    {
        array->items[i] = array->items[i + 1];
    }

    array->size--;

    // Shrink array if it's too empty (optional)
    if (array->size > 0 && array->size < array->capacity / 4)
    {
        resize_dynamic_array(array, array->capacity / 2);
    }

    return true;
}

Node *get_from_dynamic_array(DynamicArray *array, int index)
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
    {
        return false;
    }

    Node **new_data = (Node **)malloc(new_capacity * sizeof(Node));

    if (!new_data)
    {
        return false;
    }

    for (int i = 0; i < array->size; i++)
    {
        new_data[i] = array->items[i];
    }

    free(array->items);
    array->items = new_data;
    array->capacity = new_capacity;
}

bool is_dynamic_array_empty(DynamicArray *array)
{
    return array->size == 0;
}

int dynamic_array_size(DynamicArray *array)
{
    return array->size;
}