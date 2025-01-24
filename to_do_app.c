#include <ncurses/ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

// Enum for recurrence types
typedef enum
{
    NONE,
    DAILY,
    WEEKLY,
    MONTHLY,
    YEARLY
} RecurrenceType;

// Enum for completion level
typedef enum
{
    not_complete,
    in_progress,
    complete
}CompletionLevel;

// Notes structure
typedef struct Notes
{
    char *notes_description; // Dynamic string for notes description.
} Notes;

// Time structure
typedef struct Time
{
    int minutes; // Minute of the hour (0-59).
    int hours;   // Hour of the day (0-23).
} Time;

// Sub-task structure
typedef struct SubTask
{
    char *sub_task_description; // Sub-task description.
    struct SubTask *next;       // Pointer to the next sub-task (for linked list structure).
} SubTask;

// Forward declaration for Node
typedef struct Node Node;

// Task structure
typedef struct Task
{
    char *task_description;          // Dynamic string for task description.
    CompletionLevel completion_level; // Task completed or not.
    bool is_sub_task;                // Is there a sub-task or not.
    bool is_recurring;               // Is it a recurring task or not.
    bool have_reminder;              // Does it have a reminder for the task.
    unsigned int priority;           // Task priority (1- 5).
    Notes *task_notes;               // Pointer to associated notes.
    Time *task_time;                 // Pointer to associated time (e.g., deadline).
    SubTask *sub_task;               // Sub-task information (optional).
    RecurrenceType recurrence_type;  // Type of recurrence.
    char *reminder_description;      // Reminder description (if any).
    struct Appointment *appointment; // Associated appointment for the task.
} Task;

// Appointment structure
typedef struct Appointment
{
    char *time;        // Appointment time.
    char *description; // Appointment description.
    int day;           // Day of the month for the appointment.
    bool is_complete;  // New field to track completion status.
} Appointment;

// Node structure
struct Node
{
    Task task;        // storage for task for node.
    Node *head;      // Head of a node.
    Node *next;     // Pointer to the next task.
    Node *previous; // Pointer to the previous task.
    Node *tail; // tail of a node.
};

// Calendar structure
typedef struct Calendar
{
    Node *tasks;               // Linked list of tasks.
    Appointment *appointments; // Array of appointments.
    int appointment_count;     // Number of appointments.
    int year;                  // Year (e.g., 2025).
    int month;                 // Month (1 = January, 12 = December).
    int day;                   // Day of the month.
} Calendar;

// my own strdup function
char *strdup(const char *source)
{
    size_t length = strlen(source) + 1;
    char *string = (char *)malloc(length);
    if (string == NULL)
    {
        return NULL;
    }
    return (char *)memcpy(string, source, length);
}