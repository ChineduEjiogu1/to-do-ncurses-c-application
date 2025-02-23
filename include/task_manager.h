#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <stdbool.h>
#include "to_do_app.h"       // Task, Appointment, Calendar structures
#include "tree_map_api.h"    // Fast lookup with TreeMap
#include "avl_api.h"         // AVL tree for priority ordering
#include "doubly_linked_list.h" // Priority-based task and appointment management

// --- TaskManager Structure --- //
typedef struct TaskManager {
    AVL *task_tree;                  // Prioritizing tasks by urgency
    TreeMap *task_map;               // Fast lookup by task ID
    TreeMap *appointment_map;        // Fast lookup by appointment ID
    DoublyLinkedList *task_list;     // Priority-based task list
    DoublyLinkedList *appointment_list; // Priority-based appointment list
} TaskManager;

// --- Core Management Functions --- //

// TaskManager Lifecycle
TaskManager *create_task_manager();
void free_task_manager(TaskManager *manager);

// Task Management
Task *add_task(TaskManager *manager, int id, const char *description, unsigned int priority);
bool delete_task(TaskManager *manager, int task_id);
Task *update_task(TaskManager *manager, int task_id, const char *new_description, unsigned int new_priority);

// SubTask Management
SubTask *add_subtask(TaskManager *manager, int task_id, const char *description, unsigned int priority);
bool delete_subtask(TaskManager *manager, int task_id, int subtask_position);
SubTask *update_subtask(TaskManager *manager, int task_id, int subtask_position, const char *new_description, unsigned int new_priority);

// Appointment Management
Appointment *add_appointment(TaskManager *manager, int id, char date, Time time, MonthsInAYear month, bool have_reminder, RecurrenceType recurrence);
bool delete_appointment(TaskManager *manager, int appointment_id);
Appointment *update_appointment(TaskManager *manager, int appointment_id, char new_date, Time new_time, MonthsInAYear new_month, bool new_reminder, RecurrenceType new_recurrence);

// Linking Tasks to Appointments
void link_task_to_appointment(TaskManager *manager, int task_id, int appointment_id);

// Calendar Management
bool add_task_to_calendar(TaskManager *manager, Calendar *calendar, int task_id);
bool add_appointment_to_calendar(TaskManager *manager, Calendar *calendar, int appointment_id);

// --- Data Access and Retrieval --- //
Task *find_task(TaskManager *manager, int task_id);
Appointment *find_appointment(TaskManager *manager, int appointment_id);

// --- Utility Functions --- //
void print_all_tasks(TaskManager *manager);
void print_all_appointments(TaskManager *manager);

#endif // TASK_MANAGER_H
