#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H
#include <stdlib.h>
#include <stdbool.h>
typedef enum RecurrenceType
{
    NONE,
    HOURLY,
    DAILY,
    BI_WEEKLY,
    WEEKLY,
    MONTHLY,
    YEARLY
}RecurrenceType;

typedef enum MonthsInAYear
{
    JANUARY,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
} MonthsInAYear;

typedef enum CompletionLevel
{
    not_complete,
    pending,
    is_complete
}CompletionLevel;

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

typedef struct Time
{
    char minutes;
    char hour;
}Time;

typedef struct SubTask
{
    char *description;
    unsigned int priority;
}SubTask;

typedef struct Notes
{
    char *description;
}Notes;

typedef struct Appointment Appointment;

typedef struct Appointment
{
    int id;
    char date;
    Time time;
    MonthsInAYear months;
    Notes notes;
    bool is_complete;
    bool have_reminder;
    RecurrenceType recurrence_type;
    struct Task *task;  
}Appointment;

typedef struct Calendar
{
    char date;
    char year;
    Time time;
    MonthsInAYear months;
    struct Appointment *appointment;
    struct Task *task;
}Calendar;

typedef struct Task
{
    int id;
    char *description;
    bool is_complete;
    bool have_subtask;
    bool have_reminder;
    unsigned int priority;
    RecurrenceType recurrence_type;
    Time time;
    char date;
    Notes notes;
    SubTask sub_task;
    struct Appointment *appointment;
}Task;


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