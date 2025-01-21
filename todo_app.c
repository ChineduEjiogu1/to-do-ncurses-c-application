#include <ncurses.h>
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
    char *sub_task_description; // Sub-task description
    struct SubTask *next;       // Pointer to the next sub-task (for linked list structure)
} SubTask;

// Forward declaration for Node
typedef struct Node Node;

// Task structure
typedef struct Task
{
    char *task_description;          // Dynamic string for task description.
    bool is_complete;                // Task completed or not.
    bool is_sub_task;                // Is there a sub-task or not.
    bool is_recurring;               // Is it a recurring task or not.
    bool have_reminder;              // Does it have a reminder for the task.
    unsigned int priority;           // Task priority.
    Notes *task_notes;               // Pointer to associated notes.
    Time *task_time;                 // Pointer to associated time (e.g., deadline).
    SubTask *sub_task;               // Sub-task information (optional).
    RecurrenceType recurrence_type;  // Type of recurrence
    char *reminder_description;      // Reminder description (if any)
    struct Appointment *appointment; // Associated appointment for the task
} Task;

// Appointment structure
typedef struct Appointment
{
    char *time;        // Appointment time
    char *description; // Appointment description
    int day;           // Day of the month for the appointment
} Appointment;

// Node structure
struct Node
{
    Task task;      // Task stored in this node.
    Node *next;     // Pointer to the next task.
    Node *previous; // Pointer to the previous task.
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

Node *initialize_task(const char *description, bool is_complete, bool is_subtask, bool is_recurring, bool have_reminder, unsigned int priority,
                      const char *reminder_description, RecurrenceType recurrence, bool appointment, int hours, int minutes, const char *notes)
{
    Node *new_task = (Node *)malloc(sizeof(Node));
    if (!new_task)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    new_task->task.task_description = strdup(description);
    if (!new_task->task.task_description)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_task);
        exit(EXIT_FAILURE);
    }

    new_task->task.is_complete = is_complete;
    new_task->task.is_sub_task = is_subtask;
    new_task->task.is_recurring = is_recurring;
    new_task->task.recurrence_type = recurrence;
    new_task->task.priority = priority;
    new_task->task.have_reminder = have_reminder;
    new_task->task.reminder_description = have_reminder ? strdup(reminder_description) : NULL;

    new_task->task.task_time = (Time *)malloc(sizeof(Time));
    if (!new_task->task.task_time)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_task->task.task_description);
        free(new_task);
        exit(EXIT_FAILURE);
    }
    new_task->task.task_time->hours = hours;
    new_task->task.task_time->minutes = minutes;

    new_task->task.task_notes = notes ? (Notes *)malloc(sizeof(Notes)) : NULL;
    if (notes && new_task->task.task_notes)
    {
        new_task->task.task_notes->notes_description = strdup(notes);
        if (!new_task->task.task_notes->notes_description)
        {
            fprintf(stderr, "Memory allocation failed\n");
            free(new_task->task.task_notes);
            free(new_task->task.task_time);
            free(new_task->task.task_description);
            free(new_task);
            exit(EXIT_FAILURE);
        }
    }

    new_task->task.sub_task = NULL;
    new_task->task.appointment = NULL;
    new_task->next = new_task->previous = NULL;

    return new_task;
}

void free_task(Node *task)
{
    if (task->task.task_description)
    {
        free(task->task.task_description);
    }

    if (task->task.reminder_description)
    {
        free(task->task.reminder_description);
    }

    if (task->task.task_notes)
    {
        if (task->task.task_notes->notes_description)
        {
            free(task->task.task_notes->notes_description);
        }
        free(task->task.task_notes);
    }

    if (task->task.task_time)
    {
        free(task->task.task_time);
    }

    SubTask *current = task->task.sub_task;
    while (current)
    {
        SubTask *temp = current;
        current = current->next;

        if (temp->sub_task_description)
        {
            free(temp->sub_task_description);
        }
        free(temp);
    }

    if (task->task.appointment)
    {
        if (task->task.appointment->time)
        {
            free(task->task.appointment->time);
        }

        if (task->task.appointment->description)
        {
            free(task->task.appointment->description);
        }

        free(task->task.appointment);
    }

    free(task);
}

Task *create_task()
{
    Task *new_task = (Task *)malloc(sizeof(Task)); 
    if (!new_task) 
    { 
        fprintf(stderr, "Memory allocation failed\n"); 
        exit(EXIT_FAILURE); 
    }

    
}

int main()
{
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    // Your implementation to interact with tasks here.

    // End ncurses mode
    endwin();

    return 0;
}
