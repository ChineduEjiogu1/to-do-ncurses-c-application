#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "to_do_app.h"

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
    DoublyLinkedList *new_list = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));

    if (!new_list)
    {
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
        list->head = new_node;
        list->tail = new_node;
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
        list->head = new_node;
        list->tail = new_node;
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

Node *delete_by_value(DoublyLinkedList *list, int target_value)
{
    Node *current = list->head;

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
        else 
        {
            list->tail = NULL;
        }
        free(temp);
        list->size--;
        return list->head; 
    }

    if (list->tail->data->value == target_value)
    {
        Node *temp = list->tail;
        list->tail = list->tail->previous;
        if (list->tail != NULL)
        {
            list->tail->next = NULL;
        }
        else
        {
            list->head = NULL;
        }
        free(temp);
        list->size--;
        return list->head;
    }

    current = list->head;
    while (current != NULL && current->data->value != target_value)
    {
        current = current->next;
    }

    if (current != NULL) 
    {
        Node *node_to_delete = current;

        
        if (node_to_delete->previous != NULL)
        {
            node_to_delete->previous->next = node_to_delete->next;
        }
        
        if (node_to_delete->next != NULL)
        {
            node_to_delete->next->previous = node_to_delete->previous;
        }

        free(node_to_delete);
        list->size--;
    }

    return list->head;
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
    DoublyLinkedList *list = create_list(10);

    struct Data data1 = {1};
    struct Data data2 = {2};
    struct Data data3 = {3};
    struct Data data4 = {4};
    struct Data data5 = {5};
    struct Data data6 = {6};
    struct Data data7 = {7};
    struct Data data8 = {8};
    struct Data data9 = {9};
    struct Data data10 = {10};
    struct Data data11 = {11};

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
    printf("After inserting 7 after second node: \n");
    traverse_forward(list);

    insert_after(list, fifth_node, &data6);
    printf("After inserting 6 after fifth node: \n");
    traverse_forward(list);

    insert_before(list, first_node, &data8);
    printf("After inserting 8 at the head: \n");
    traverse_forward(list);

    insert_before(list, fourth_node, &data9);
    printf("After inserting 9 before fourth node: \n");
    traverse_forward(list);

    int target_value = 3;
    delete_by_value(list, target_value);
    printf("After deleting node with value %d: ", target_value);
    traverse_forward(list);

    int target_value_2 = 5;
    delete_by_value(list, target_value_2);
    printf("After deleting node with value %d: ", target_value_2);
    traverse_forward(list);

    insert_front(list, &data10);
    printf("Inserting 10 in front of the list: \n");
    traverse_forward(list);

    insert_back(list, &data11);
    printf("Inserting 11 at the back of the list: \n");
    traverse_forward(list);

    free_list(list);
    return 0;
}