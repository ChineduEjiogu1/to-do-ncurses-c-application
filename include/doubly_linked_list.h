#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

typedef struct Node
{
    struct Data *data;
    struct Node *previous;
    struct Node *next;
}Node;

typedef struct Data
{
    void* value;
}Data;

typedef struct DoublyLinkedList
{
    Node *head;
    Node *tail;
    unsigned int size;
    int max_capacity;
}DoublyLinkedList;

Node* create_node(struct Data *data);

DoublyLinkedList *create_list(int capacity);

bool is_empty(DoublyLinkedList *list);

bool is_full(DoublyLinkedList *list);

int list_size(DoublyLinkedList *list);

Node *insert_front(DoublyLinkedList *list, struct Data *data);

Node *insert_back(DoublyLinkedList *list, struct Data *data);

Node *insert_by_position(DoublyLinkedList *list, unsigned int position, struct Data *data);

Node *insert_after(DoublyLinkedList *list, Node *previous_node, struct Data *data);

Node *insert_before(DoublyLinkedList *list, Node* next_node, struct Data *data);

Node *delete_by_value(DoublyLinkedList *list, int target_value);

Node *search_list(DoublyLinkedList *list, int target_value);

void traverse_forward(DoublyLinkedList *list);

void free_list(DoublyLinkedList *list);

#endif