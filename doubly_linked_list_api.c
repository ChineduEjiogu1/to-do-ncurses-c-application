#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "doubly_linked_list.h"
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

Node *delete_by_value(DoublyLinkedList *list, int target_value)
{
    if (!list || is_empty(list))
    {
        printf("List is empty or invalid\n");
        return NULL;
    }

    Node *current = list->head;

    if ((int)(intptr_t)list->head->data->value == target_value)
    {
        Node *node_to_delete = list->head;
        Node *new_head = list->head->next;

        if (new_head)
        {
            new_head->previous = NULL;
        }
        else
        {
            list->tail = NULL;
        }
        free(node_to_delete->data);
        free(node_to_delete);
        list->head = new_head;
        list->size--;
        return new_head;
    }

    while (current && (int)(intptr_t)current->data->value != target_value)
    {
        current = current->next;
    }

    if (!current)
    {
        printf("Value %d not found in list\n", target_value);
        return list->head;
    }

    Node *node_to_delete = current;

    node_to_delete->previous->next = node_to_delete->next;

    if (node_to_delete->next)
    {
        node_to_delete->next->previous = node_to_delete->previous;
    }
    else
    {
        list->tail = node_to_delete->previous;
    }

    free(node_to_delete->data);
    free(node_to_delete);
    list->size--;
    return list->head;
}

Node *search_list(DoublyLinkedList *list, int target_value)
{
    if (!list)
    {
        printf("Invalid list\n");
        return NULL;
    }

    Node *current = list->head;

    while (current)
    {
        if ((int)(intptr_t)current->data->value == target_value)
        {
            printf("Found node with value %d at address %p\n", target_value, (void *)current);
            return current;
        }
        current = current->next;
    }

    printf("Value %d not found in the list\n", target_value);
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
        printf("%d <-> ", current->data->value);
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

int main()
{
    DoublyLinkedList *list = create_list(MAX_CAPACITY);

    struct Data *data1 = (struct Data *)malloc(sizeof(struct Data));
    data1->value = (void *)1;

    struct Data *data2 = (struct Data *)malloc(sizeof(struct Data));
    data2->value = (void *)2;

    struct Data *data3 = (struct Data *)malloc(sizeof(struct Data));
    data3->value = (void *)3;

    struct Data *data4 = (struct Data *)malloc(sizeof(struct Data));
    data4->value = (void *)4;

    struct Data *data5 = (struct Data *)malloc(sizeof(struct Data));
    data5->value = (void *)5;

    struct Data *data6 = (struct Data *)malloc(sizeof(struct Data));
    data6->value = (void *)6;

    struct Data *data7 = (struct Data *)malloc(sizeof(struct Data));
    data7->value = (void *)7;

    struct Data *data8 = (struct Data *)malloc(sizeof(struct Data));
    data8->value = (void *)8;

    struct Data *data9 = (struct Data *)malloc(sizeof(struct Data));
    data9->value = (void *)9;

    struct Data *data10 = (struct Data *)malloc(sizeof(struct Data));
    data10->value = (void *)10;

    struct Data *data11 = (struct Data *)malloc(sizeof(struct Data));
    data11->value = (void *)11;

    insert_by_position(list, 0, data1);
    insert_by_position(list, 1, data3);
    insert_by_position(list, 2, data2);
    insert_by_position(list, 3, data4);
    insert_by_position(list, 4, data5);

    printf("Original list before any additions:\n");
    traverse_forward(list);

    Node *first_node = list->head;
    Node *second_node = list->head->next;
    Node *fourth_node = list->head->next->next->next;
    Node *fifth_node = list->head->next->next->next->next;

    insert_after(list, second_node, data7);
    printf("After inserting 7 after second node: \n");
    traverse_forward(list);

    insert_after(list, fifth_node, data6);
    printf("After inserting 6 after fifth node: \n");
    traverse_forward(list);

    insert_before(list, first_node, data8);
    printf("After inserting 8 at the head: \n");
    traverse_forward(list);

    insert_before(list, fourth_node, data9);
    printf("After inserting 9 before fourth node: \n");
    traverse_forward(list);

    int target_value = 3;
    list->head = delete_by_value(list, target_value);
    printf("After deleting node with value %d: \n", target_value);
    traverse_forward(list);

    int target_value_2 = 5;
    list->head = delete_by_value(list, target_value_2);
    printf("After deleting node with value %d: \n", target_value_2);
    traverse_forward(list);

    insert_front(list, data10);
    printf("Inserting 10 in front of the list: \n");
    traverse_forward(list);

    insert_back(list, data11);
    printf("Inserting 11 at the back of the list: \n");
    traverse_forward(list);

    printf("Testing is_empty on empty list: %s\n",
           is_empty(list) ? "true" : "false");

    // Test is_full on empty list
    printf("Testing is_full on empty list: %s\n",
           is_full(list) ? "true" : "false");

    printf("List size is: %d\n", list_size(list));

    free_list(list);
    return 0;
}