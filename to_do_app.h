#ifndef TO_DO_APP_H
#define TO_DO_APP_H

#include "doubly_linked_list.h"
#include "hash_map.h"
#include "bst.h"

// Recurrence Types
typedef enum RecurrenceType {
    NONE,
    HOURLY,
    DAILY,
    BI_WEEKLY,
    WEEKLY,
    MONTHLY,
    YEARLY
} RecurrenceType;

// Months in a Year
typedef enum MonthsInAYear {
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

// Completion Levels
typedef enum CompletionLevel {
    NOT_COMPLETE,
    PENDING,
    IS_COMPLETE
} CompletionLevel;

// Time Structure
typedef struct Time {
    char minutes;
    char hour;
} Time;

// Notes (Doubly Linked List will be used for multiple notes)
typedef struct Notes {
    char *description;
} Notes;

// SubTask Structure
typedef struct SubTask {
    int task_id; // Reference to parent Task
    char *description;
    unsigned int priority;
} SubTask;

// Appointment Structure
typedef struct Appointment {
    int id;
    char date;
    Time time;
    MonthsInAYear month;
    DoublyLinkedList *notes; // Multiple notes
    bool is_complete;
    bool have_reminder;
    RecurrenceType recurrence_type;
    struct Task *task;  // Linked Task
} Appointment;

// Task Structure
typedef struct Task {
    int id;
    char *description;
    bool is_complete;
    bool have_subtask;
    bool have_reminder;
    unsigned int priority;
    RecurrenceType recurrence_type;
    Time time;
    char date;
    DoublyLinkedList *notes; // Multiple notes
    DoublyLinkedList *subtasks; // Multiple subtasks
    Appointment *appointment; // Linked Appointment
} Task;

// Calendar Structure (Stores Multiple Tasks & Appointments)
typedef struct Calendar {
    char date;
    char year;
    Time time;
    MonthsInAYear month;
    DoublyLinkedList *appointments; // Store multiple appointments
    DoublyLinkedList *tasks;        // Store multiple tasks
} Calendar;

// Function Prototypes

// Task Management
Task *create_task(BST *task_tree, DoublyLinkedList *list, HashMap *map, int id, const char *description, unsigned int priority);
Task *update_task(BST *task_bst, HashMap *map, int task_id, const char *new_description, unsigned int new_priority,
    void (*free_data)(void *), void (*free_key)(void *), int (*compare_keys)(void *, void *),
    void *(*copy_key)(void *, size_t), void *(*copy_data)(void *, size_t));
bool delete_task(DoublyLinkedList *list, HashMap *map, int task_id);

// SubTask Management
SubTask *create_subtask(DoublyLinkedList *list, HashMap *map, int task_id, const char *description, unsigned int priority);
SubTask *update_subtask(BST *subtask_bst, HashMap *map, int task_id, const char *new_description, unsigned int new_priority,
    void (*free_data)(void *), void (*free_key)(void *), int (*compare_keys)(void *, void *),
    void *(*copy_key)(void *, size_t), void *(*copy_data)(void *, size_t));
bool delete_sub_task(DoublyLinkedList *list, HashMap *map, int task_id);

// Appointment Management
Appointment *create_appointment(DoublyLinkedList *list, HashMap *map, int id, char date, Time time, MonthsInAYear month, bool have_reminder, RecurrenceType recurrence);
Appointment *update_appointment(DoublyLinkedList *list, HashMap *map, int appointment_id, char new_date, Time new_time, MonthsInAYear new_month, bool new_have_reminder, RecurrenceType new_recurrence);
bool delete_appointment(DoublyLinkedList *list, HashMap *map, int appointment_id);
void link_task_to_appointment(Task *task, Appointment *appointment);

// Calendar Management
Calendar *create_calendar(char date, char year, Time time, MonthsInAYear month);
void add_task_to_calendar(Calendar *calendar, BST *task_bst, HashMap *map, int task_id);
Node *add_appointment_to_calendar(Calendar *calendar, DoublyLinkedList *list, HashMap *map, int appointment_id);
Calendar *update_calendar(Calendar *calendar, char date, char year, Time time, MonthsInAYear month);

#endif