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
    char *sub_task_description;     // Sub-task description.
    struct SubTask *previous;     // Pointer to the previous sub-task (for linked list structure).
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
    int appointment_duration; // how long appointment will last.
    struct Task *task;
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

Node *create_task(const char *task_description, CompletionLevel completion_level, bool is_subtask, bool is_recurring, bool have_reminder, 
                    unsigned int priority, RecurrenceType recurrence_type, Time *minutes, Time *hours, const char *notes_description, 
                    const char *reminder_description, SubTask *sub_task)
{
    Node *new_task = (Node *)malloc(sizeof(Node));

    if (new_task == NULL)
    {
        return NULL;
    }

    new_task->task.task_description = NULL;
    new_task->task.task_notes->notes_description = NULL;
    new_task->task.reminder_description = NULL;
    new_task->task.sub_task = NULL;

    if (task_description != NULL)
    {
        new_task->task.task_description = strdup(task_description);

        if (new_task->task.task_description)
        {
            free(new_task->task.task_description);
        }
    }

    if (notes_description != NULL)
    {
        new_task->task.task_notes->notes_description = strdup(notes_description);

        if (new_task->task.task_notes)
        {
            free(new_task->task.task_notes);
        }
        
        free(new_task->task.task_notes->notes_description);
    }

    if (reminder_description != NULL)
    {
        new_task->task.reminder_description = strdup(reminder_description);

        if (new_task->task.reminder_description)
        {
            free(new_task->task.reminder_description);
        }
    }

    if (sub_task != NULL)
    {
        new_task->task.sub_task = sub_task;
        free(sub_task);
    }
   
    new_task->task.have_reminder = have_reminder;
    new_task->task.is_recurring = is_recurring;
    new_task->task.is_sub_task = is_recurring;
    new_task->task.priority = priority;
    new_task->task.recurrence_type = recurrence_type;
    new_task->task.task_time = hours;
    new_task->task.task_time = minutes;
    new_task->task.completion_level = completion_level;

    return new_task;
}

Calendar *create_calendar(Node *task, int day, int month, int year)
{
    Calendar *new_calendar = (Calendar *)malloc(sizeof(Calendar));

    new_calendar->day = day;
    new_calendar->month = month;
    new_calendar->year = year;
    new_calendar->tasks = task;

    return new_calendar;
}

Appointment *create_appointment(int day, const char *appointment_description, bool is_complete, int appointment_duration, const char *time)
{
    Appointment *new_appointment = (Appointment *)malloc(sizeof(Appointment));

    new_appointment->day = day;
    
    if (appointment_description != NULL)
    {

    }

    new_appointment->appointment_duration;
    new_appointment->time;

    return new_appointment;
}

int main()
{
    return 0;
}