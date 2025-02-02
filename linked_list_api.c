#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "to_do_app.h"

Node* create_node(struct Data *data)
{
    Node* new_node = (Node *)malloc(sizeof(Node));

    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->previous = new_node->next = NULL;
    new_node->data = data;

    return new_node;
}

DoublyLinkedList *create_list()
{
    DoublyLinkedList *list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));

    if (list == NULL)
    {
        return NULL;
    }

    list->head = list->tail = NULL;

    return list;
}

bool is_empty(DoublyLinkedList *list)
{
    return list->head == NULL && list->tail == NULL;
}

Node *insert_by_position(DoublyLinkedList *list, unsigned int position, struct Data *data)
{
    Node *new_node = create_node(data);

    if (position == 1)
    {
        new_node->next = list->head;

        if (list->head != NULL)
        {
            list->head->previous = new_node;
        }

        list->head = new_node;

        return list->head;
    }

    Node *current = list->head;

    for (int i = 1; i < position -  1 && current != NULL; i++)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("Position is out of bounds.\n");
        free(new_node);
        return list->head;
    }

    new_node->previous = current;

    new_node->next = current->next;

    current->next = new_node;

    if (new_node->next != NULL)
    {
        new_node->next->previous = new_node;
    }
    
    return list->head;
}

void insert_after(Node* previous_node, struct Data *data)
{
    if (previous_node == NULL)
    {
        printf("The previous node cannot be NULL\n");
        return;
    }

    Node *new_node = create_node(data);

    new_node->next = previous_node->next;
    new_node->previous = previous_node;

    if (previous_node->next != NULL)
    {
        previous_node->next->previous = new_node;
    }

    previous_node->next = new_node;
}

void insert_before(DoublyLinkedList *list, Node* next_node, struct Data *data)
{
    if (next_node == NULL)
    {
        printf("The given next cannot be NULL\n");
        return;
    }

    Node *new_node = create_node(data);

    new_node->previous = next_node->previous;

    new_node->next = next_node;

    if (next_node->previous != NULL)
    {
        next_node->previous->next = new_node;
    }
    else
    {
        list->head = new_node;
    }

    next_node->previous = new_node;
}

Node *delete_by_value(DoublyLinkedList *list, int target_value)
{
    if (list->head == NULL)
    {
        return NULL;
    }

    if (list->head->data->value == target_value)
    {
       Node *temp = list->head;
       list->head = list->head->next;

       if (list->head != NULL)
       {
            list->head->previous = NULL;
       }

       free(temp);
       return list->head;
    }

    Node *current = list->head;
    
    while (current != NULL && current->data->value != target_value)
    {
       current = current->next;
    }

    if (current != NULL)
    {
        Node *node_to_delete = current->next;
        current->next = node_to_delete->next;

        if (node_to_delete->next != NULL)
        {
            node_to_delete->next->previous = current;
        }
        
        free(node_to_delete);
    }

    return list->head;
}

void traverse_forward(DoublyLinkedList *list)
{
    Node *temp = list->head;

    while (temp != NULL)
    {
        printf("%d ", temp->data->value);
        temp = temp->next;
    }

    printf("\n");
}

void free_node(DoublyLinkedList *list)
{
    Node *temp;

    while (list->head != NULL)
    {
        temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
}


int main() {
    DoublyLinkedList *list = create_list();

    // Create some data nodes
    struct Data data1 = {1};
    struct Data data2 = {2};
    struct Data data3 = {3};
    struct Data data4 = {4};
    struct Data data5 = {5};

    // Insert nodes using insert_by_position
    insert_by_position(list, 1, &data1);
    insert_by_position(list, 2, &data2);
    insert_by_position(list, 3, &data3);

    printf("Original list: ");
    traverse_forward(list);

    // Insert a node after a specific node using insert_after
    Node *second_node = list->head->next;
    insert_after(second_node, &data4);
    printf("After inserting 4 after second node: ");
    traverse_forward(list);

    // Insert a node before a specific node using insert_before
    insert_before(list, second_node, &data5);
    printf("After inserting 5 before second node: ");
    traverse_forward(list);

    // Delete a node by value using delete_by_value
    int target_value = 3;
    delete_by_value(list, target_value);
    printf("After deleting node with value %d: ", target_value);
    traverse_forward(list);

    // Check if the list is empty using is_empty
    bool empty = is_empty(list);
    printf("Is the list empty? %s\n", empty ? "Yes" : "No");

    // Free the nodes and the list using free_node
    free_node(list);
    free(list);

    return 0;
}