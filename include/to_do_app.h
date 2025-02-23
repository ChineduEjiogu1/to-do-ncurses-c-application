#ifndef TO_DO_APP_H
#define TO_DO_APP_H

#include <stdbool.h>
#include "doubly_linked_list.h"

// --- ENUMS --- //

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

// Months of the Year
typedef enum MonthsInAYear {
    JANUARY, FEBRUARY, MARCH, APRIL, MAY, JUNE,
    JULY, AUGUST, SEPTEMBER, OCTOBER, NOVEMBER, DECEMBER
} MonthsInAYear;

// Task Completion Levels
typedef enum CompletionLevel {
    NOT_COMPLETE,
    PENDING,
    IS_COMPLETE
} CompletionLevel;

// --- STRUCTS --- //

// Time Structure
typedef struct Time {
    char minutes;
    char hour;
} Time;

// Notes Structure
typedef struct Notes {
    char *description;
} Notes;

// SubTask Structure
typedef struct SubTask {
    int task_id; // Linked parent task ID
    char *description;
    unsigned int priority; // Subtask priority within the task
} SubTask;

// Appointment Structure
typedef struct Appointment {
    int id;
    char date;
    Time time;
    MonthsInAYear month;
    DoublyLinkedList *notes;
    bool is_complete;
    bool have_reminder;
    RecurrenceType recurrence_type;
    struct Task *linked_task; // Points back to linked task
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
    Notes *note; // A single note attached to the task
    DoublyLinkedList *subtasks; // List of subtasks
    Appointment *appointment; // Link to an appointment
} Task;

// Calendar Structure
typedef struct Calendar {
    char date;
    char year;
    Time time;
    MonthsInAYear month;
    DoublyLinkedList *appointments; // List of appointments
    DoublyLinkedList *tasks;        // List of tasks
} Calendar;

#endif // TO_DO_APP_H
