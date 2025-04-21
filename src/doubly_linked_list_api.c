#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "../include/doubly_linked_list.h"
#include "../include/lru_cache_api.h"
#include "../include/tree_map_api.h"

#define MAX_CAPACITY 1000

Node *create_node(struct Data *data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    if (!new_node)
    {
        printf("Memory allocation failed for node\n");
        return NULL;
    }

    new_node->previous = new_node->next = NULL;
    new_node->data = data;

    return new_node;
}

DoublyLinkedList *create_list(int capacity)
{
    if (capacity > MAX_CAPACITY)
    {
        printf("Capacity exceeds maximum limit of %d\n", MAX_CAPACITY);
        return NULL;
    }

    DoublyLinkedList *new_list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));

    if (!new_list)
    {
        printf("Memory allocation failed for list\n");
        return NULL;
    }

    new_list->head = new_list->tail = NULL;
    new_list->size = 0;
    new_list->max_capacity = capacity;

    return new_list;
}

bool is_empty(DoublyLinkedList *list)
{
    return list->head == NULL && list->size == 0;
}

bool is_full(DoublyLinkedList *list)
{
    return list->size >= list->max_capacity;
}

int list_size(DoublyLinkedList *list)
{
    if (!list)
    {
        printf("Invalid list\n");
        return 0;
    }

    return list->size;
}

Node *insert_front(DoublyLinkedList *list, struct Data *data)
{
    if (!list || !data || is_full(list))
    {
        printf("Invalid parameters or list is full\n");
        return NULL;
    }

    Node *new_node = create_node(data);

    if (!new_node)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    if (is_empty(list))
    {
        list->head = list->tail = new_node;
    }
    else
    {
        new_node->next = list->head;
        list->head->previous = new_node;
        list->head = new_node;
    }

    list->size++;
    return new_node;
}

Node *insert_back(DoublyLinkedList *list, struct Data *data)
{
    if (!list || !data || is_full(list))
    {
        printf("Invalid parameters or list is full\n");
        return NULL;
    }

    Node *new_node = create_node(data);

    if (!new_node)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    if (is_empty(list))
    {
        list->head = list->tail = new_node;
    }
    else
    {
        new_node->previous = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return new_node;
}

Node *insert_by_position(DoublyLinkedList *list, unsigned int position, struct Data *data)
{
    if (!list || !data || is_full(list))
    {
        printf("Invalid parameters or list is full\n");
        return NULL;
    }

    if (position > list->size)
    {
        printf("Position is out of bounds\n");
        return NULL;
    }

    if (position == 0)
    {
        return insert_front(list, data);
    }

    if (position == list->size)
    {
        return insert_back(list, data);
    }

    Node *new_node = create_node(data);
    if (!new_node)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    Node *current = list->head;
    for (unsigned int i = 0; i < position - 1; i++)
    {
        current = current->next;
    }

    new_node->next = current->next;
    new_node->previous = current;
    current->next->previous = new_node;
    current->next = new_node;
    list->size++;

    return new_node;
}

Node *insert_after(DoublyLinkedList *list, Node *previous_node, struct Data *data)
{
    if (!list || !data || is_full(list))
    {
        printf("Invalid parameters or list is full");
        return NULL;
    }

    Node *new_node = create_node(data);

    Node *current = previous_node;

    if (current == NULL)
    {
        printf("Previous node cannot be NULL");
        return NULL;
    }

    if (!new_node)
    {
        printf("Memory allocation failed");
        return NULL;
    }

    if (current == list->tail)
    {
        list->tail->next = new_node;
        new_node->previous = list->tail;
        list->tail = new_node;
        new_node->next = NULL;
        list->size++;
        return new_node;
    }

    if (current == list->head)
    {
        new_node->next = list->head->next;
        if (list->head->next != NULL)
        {
            list->head->next->previous = new_node;
        }

        list->head->next = new_node;
        new_node->previous = list->head;
        list->size++;
        return new_node;
    }

    new_node->next = current->next;

    if (current->next != NULL)
    {
        current->next->previous = new_node;
    }

    current->next = new_node;
    new_node->previous = current;
    list->size++;

    return new_node;
}

Node *insert_before(DoublyLinkedList *list, Node *next_node, struct Data *data)
{
    if (!list || !data || is_full(list))
    {
        printf("Invalid parameters or list is full");
        return NULL;
    }

    Node *new_node = create_node(data);

    Node *current = next_node;

    if (current == NULL)
    {
        printf("Next node cannot be NULL");
        return NULL;
    }

    if (!new_node)
    {
        printf("Memory allocation failed");
        return NULL;
    }

    if (is_empty(list))
    {
        list->head = new_node;
        list->tail = new_node;
        new_node->previous = NULL;
        new_node->next = NULL;
        list->size++;
        return new_node;
    }

    if (current == list->head)
    {
        new_node->next = list->head;
        new_node->previous = NULL;
        list->head->previous = new_node;
        list->head = new_node;
        list->size++;
        return new_node;
    }

    if (current == list->tail)
    {
        new_node->previous = list->tail->previous;
        list->tail->previous = new_node;
        new_node->next = list->tail;
        list->tail->previous->next = new_node;
        list->size++;
        return new_node;
    }

    new_node->next = current;
    new_node->previous = current->previous;

    if (current->previous != NULL)
    {
        current->previous->next = new_node;
    }
    current->previous = new_node;
    list->size++;

    return new_node;
}

Node *delete_by_value(DoublyLinkedList *list, void *target_value)
{
    if (!list)
    {
        printf("delete_by_value: list is NULL\n");
        return NULL;
    }

    if (is_empty(list))
    {
        printf("delete_by_value: list is empty\n");
        return NULL;
    }

    if (!target_value)
    {
        printf("delete_by_value: target_value is NULL\n");
        return list->head;
    }

    Node *current = list->head;

    while (current)
    {
        // Remove or comment out the printf line below to stop printing pointers
        // printf("Visiting node: %p, data: %p, value: %p\n",
        //        current, current->data, current->data ? current->data->value : NULL);

        if (current->data && current->data->value == target_value)
        {
            Node *node_to_delete = current;

            if (node_to_delete == list->head)
            {
                list->head = node_to_delete->next;
                if (list->head)
                    list->head->previous = NULL;
                else
                    list->tail = NULL;
            }
            else if (node_to_delete == list->tail)
            {
                list->tail = node_to_delete->previous;
                if (list->tail)
                    list->tail->next = NULL;
            }
            else
            {
                if (node_to_delete->previous)
                    node_to_delete->previous->next = node_to_delete->next;
                if (node_to_delete->next)
                    node_to_delete->next->previous = node_to_delete->previous;
            }

            printf("Deleted node with key=%d from DLL\n",
                   void_ptr_to_int(((LRUNode *)target_value)->key));

            if (node_to_delete->data)
                free(node_to_delete->data);
            free(node_to_delete);
            list->size--;

            return list->head;
        }

        current = current->next;
    }

    // printf("Value with key=%d not found in DLL\n",
    //        void_ptr_to_int(((LRUNode *)target_value)->key));
    return list->head;
}

Node *search_list(DoublyLinkedList *list, void *target_value)
{
    if (!list)
    {
        printf("Invalid list\n");
        return NULL;
    }

    int target = void_ptr_to_int(target_value);
    Node *current = list->head;

    while (current)
    {
        int val = void_ptr_to_int(current->data->value);
        if (val == target)
        {
            printf("Found node with value %d at address %p\n", val, (void *)current);
            return current;
        }
        current = current->next;
    }

    printf("Value %d not found in the list\n", target);
    return NULL;
}

void traverse_forward(DoublyLinkedList *list)
{
    if (!list || is_empty(list))
    {
        printf("List is empty\n");
        return;
    }

    Node *current = list->head;

    while (current)
    {
        printf("%d <-> ", void_ptr_to_int(current->data->value));
        current = current->next;
    }

    printf("NULL\n");
}

void free_list(DoublyLinkedList *list)
{
    if (!list)
        return;

    Node *temp = list->head;

    while (temp != NULL)
    {
        Node *next_node = temp->next;
        free(temp->data);
        free(temp);
        temp = next_node;
    }

    free(list);
}

// int main() {
//     // Create a new doubly linked list with a max capacity of 10
//     DoublyLinkedList *list = create_list(10);

//     // Check if the list was created successfully
//     if (!list) {
//         printf("Failed to create list\n");
//         return -1;
//     }

//     // Create sample data for the nodes
//     struct Data *data1 = (struct Data *)malloc(sizeof(struct Data));
//     data1->value = int_to_void_ptr(10);

//     struct Data *data2 = (struct Data *)malloc(sizeof(struct Data));
//     data2->value = int_to_void_ptr(20);

//     struct Data *data3 = (struct Data *)malloc(sizeof(struct Data));
//     data3->value = int_to_void_ptr(30);

//     // Insert nodes into the list
//     insert_front(list, data1); // Insert 10 at the front
//     insert_back(list, data2);  // Insert 20 at the back
//     insert_by_position(list, 1, data3); // Insert 30 at position 1

//     // Traverse the list
//     printf("List after insertions:\n");
//     traverse_forward(list);

//     // Search for a node with value 20
//     int target = 20;
//     printf("\nSearching for value %d in the list...\n", target);
//     search_list(list, int_to_void_ptr(target));

//     // Delete a node with value 10
//     printf("\nDeleting value 10 from the list...\n");
//     delete_by_value(list, int_to_void_ptr(10));
    
//     // Traverse the list after deletion
//     printf("\nList after deletion:\n");
//     traverse_forward(list);

//     // Free memory and clean up
//     free_list(list);

//     return 0;
// }
