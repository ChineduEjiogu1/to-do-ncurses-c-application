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
    int value;
}Data;

typedef struct DoublyLinkedList
{
    Node *head;
    Node *tail;
    unsigned int size;
    int max_capacity;
}DoublyLinkedList;


typedef enum ListError {
    LIST_SUCCESS,
    LIST_MEMORY_ERROR,
    LIST_INVALID_LIST,
    LIST_INVALID_POSITION,
    LIST_FULL,
    LIST_EMPTY
} ListError;

Node* create_node(struct Data *data);

DoublyLinkedList *create_list();

bool is_empty(DoublyLinkedList *list);

bool is_full(DoublyLinkedList *list);

Node *insert_front(DoublyLinkedList *list, struct Data *data);

Node *insert_back(DoublyLinkedList *list, struct Data *data);

Node *insert_by_position(DoublyLinkedList *list, unsigned int position, struct Data *data);

Node *insert_after(DoublyLinkedList *list, Node *previous_node, struct Data *data);

Node *insert_before(DoublyLinkedList *list, Node* next_node, struct Data *data);

Node *delete_by_value(DoublyLinkedList *list, int target_value);

void traverse_forward(DoublyLinkedList *list);

void free_list(DoublyLinkedList *list);

#endif