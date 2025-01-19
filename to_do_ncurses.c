#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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

// Forward declaration for Node
typedef struct Node Node;

// Task structure
typedef struct Task
{
    char *task_description; // Dynamic string for task description.
    bool is_complete;       // Task completed or not.
    bool is_sub_task;       // Is there a sub-task or not.
    bool is_recurring;      // Is it a recurring task or not.
    bool have_reminder;     // Does it have a reminder for the task.
    unsigned int priority;  // Task priority.
    Notes *task_notes;      // Pointer to associated notes.
    Time *task_time;        // Pointer to associated time (e.g., deadline).
} Task;

// Node structure
struct Node
{
    Task task;       // Task stored in this node.
    Node *next;      // Pointer to the next task.
    Node *previous;  // Pointer to the previous task.
};

// Calendar structure
typedef struct Calendar
{
    Node *tasks; // Linked list of tasks.
    int year;    // Year (e.g., 2025).
    int month;   // Month (1 = January, 12 = December).
    int day;     // Day of the month.
    Time *day_time; // Pointer to time details for the day.
} Calendar;

// Create Notes
Notes *create_notes(const char *notes_description)
{
    Notes *new_notes = (Notes *)malloc(sizeof(Notes));
    if (new_notes)
    {
        new_notes->notes_description = strdup(notes_description);
    }
    return new_notes;
}

// Free Notes
void free_notes(Notes *notes)
{
    if (notes)
    {
        free(notes->notes_description);
        free(notes);
    }
}

// Create Node
Node *create_node(const char *description, bool is_complete, bool is_sub_task, bool is_recurring, bool have_reminder, unsigned int priority, const char *notes_description, Time *task_time)
{
    Node *new_node = (Node *)malloc(sizeof(Node));

    if (description)
    {
        new_node->task.task_description = (char *)malloc(strlen(description) + 1);
        strcpy(new_node->task.task_description, description);
    }
    else
    {
        new_node->task.task_description = NULL;
    }

    new_node->task.is_complete = false;
    new_node->task.is_recurring = is_recurring;
    new_node->task.is_sub_task = is_sub_task;
    new_node->task.priority = priority;
    new_node->task.have_reminder = false;
    new_node->task.task_time = task_time;

    if (notes_description)
    {
        new_node->task.task_notes = create_notes(notes_description);
    }
    else
    {
        new_node->task.task_notes = NULL;
    }

    new_node->next = new_node->previous = NULL;

    return new_node;
}

// Free Node
void free_node(Node *node)
{
    if (node)
    {
        free(node->task.task_description); // Free task description
        free_notes(node->task.task_notes); // Free associated notes
        free(node);                        // Free the node itself
    }
}


int main()
{
    return 0;
}