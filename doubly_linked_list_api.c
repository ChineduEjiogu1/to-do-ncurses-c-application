#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "to_do_app.h"

Node *create_node(struct Data *data)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    if (!new_node)
    {
        return NULL;
    }

    new_node->previous = new_node->next = NULL;
    new_node->data = data;

    return new_node;
}

DoublyLinkedList *create_list()
{
    DoublyLinkedList *new_list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));

    if (!new_list)
    {
        return NULL;
    }

    new_list->head = new_list->tail = NULL;
    new_list->size = 0;

    return new_list;
}

bool is_empty(DoublyLinkedList *list)
{
    return list->head == NULL && list->size == 0;
}

Node *insert_by_position(DoublyLinkedList *list, unsigned int position, struct Data *data)
{
    // Check if list is NULL
    if (!list)
    {
        printf("List pointer is NULL");
        return NULL;
    }

    // Check if data is NULL
    if (!data)
    {
        printf("Data pointer is NULL");
        return NULL;
    }

    // Check if position is valid
    if (position > list->size)
    {
        printf("Position is out of bounds");
        return NULL;
    }

    Node *new_node = create_node(data);
    if (!new_node)
    {
        printf("Memory allocation failed");
        return NULL;
    }

    // Handle empty list
    if (is_empty(list))
    {
        list->head = new_node;
        list->tail = new_node;
        new_node->previous = NULL;
        new_node->next = NULL;
        list->size++;
        return new_node;
    }

    // Handle position 0
    if (position == 0)
    {
        new_node->next = list->head;
        new_node->previous = NULL;
        list->head->previous = new_node;
        list->head = new_node;
        list->size++;
        return new_node;
    }

    // Handle position equals size
    if (position == list->size)
    {
        new_node->previous = list->tail;
        new_node->next = NULL;
        list->tail->next = new_node;
        list->tail = new_node;
        list->size++;
        return new_node;
    }

    // Find insertion point
    Node *current = list->head;
    unsigned int current_position = 0;

    while (current != NULL && current_position < position - 1)
    {
        current = current->next;
        current_position++;
    }

    if (current == NULL || current_position >= position - 1)
    {
        free(new_node);
        printf("List position is out of bounds");
        return NULL;
    }

    // Insert node
    new_node->next = current->next;
    new_node->previous = current;
    current->next->previous = new_node;
    current->next = new_node;
    list->size++;

    return new_node;
}

Node *insert_after(DoublyLinkedList *list, Node *previous_node, struct Data *data)
{
    if (!list || !data)
    {
        printf("Invalid parameters");
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
    if (!list || !data)
    {
        printf("Invalid parameters");
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

void traverse_forward(DoublyLinkedList *list)
{
    Node *temp = list->head;

    while (temp != NULL)
    {
        printf("%d <-> ", temp->data->value);
        temp = temp->next;
    }

    printf("NULL\n");
}

void free_list(DoublyLinkedList *list)
{
    Node *temp = list->head;

    while (temp != NULL)
    {
        Node *next_node = temp->next;
        free(temp);
        temp = next_node;
    }

    free(list);
}

int main()
{
    DoublyLinkedList *list = create_list();

    struct Data *data = (Data *)malloc(sizeof(struct Data));

    struct Data data1 = {1};
    struct Data data2 = {2};
    struct Data data3 = {3};
    struct Data data4 = {4};
    struct Data data5 = {5};
    struct Data data6 = {6};
    struct Data data7 = {7};

    struct Data data8 = {8};
    struct Data data9 = {9};

    insert_by_position(list, 0, &data1);
    insert_by_position(list, 1, &data3);
    insert_by_position(list, 2, &data2);
    insert_by_position(list, 3, &data4);
    insert_by_position(list, 4, &data5);

    printf("Original list before any additions:\n");
    traverse_forward(list);

    Node *first_node = list->head;
    Node *second_node = list->head->next;
    Node *fourth_node = list->head->next->next->next;
    Node *fifth_node = list->head->next->next->next->next;

    insert_after(list, second_node, &data7);
    printf("After inserting 7 after second node: ");
    traverse_forward(list);

    insert_after(list, fifth_node, &data6);
    printf("After inserting 6 after fifth node: ");
    traverse_forward(list);

    insert_before(list, first_node, &data8);
    printf("After inserting 8 at the head: ");
    traverse_forward(list);

    insert_before(list, fourth_node, &data9);
    printf("After inserting 9 before fourth node: ");
    traverse_forward(list);

    free_list(list);
    return 0;
}