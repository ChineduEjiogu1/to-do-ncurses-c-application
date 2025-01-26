#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_YEAR 9999

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
} CompletionLevel;

// Enum for month of a year.
typedef enum
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
} Month;

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
    struct SubTask *previous;   // Pointer to the previous sub-task (for linked list structure).
    struct SubTask *next;       // Pointer to the next sub-task (for linked list structure).
} SubTask;

// Appointment structure
typedef struct Appointment
{
    int id;
    char *time;               // Appointment time.
    char *description;        // Appointment description.
    int day;                  // Day of the month for the appointment.
    bool is_complete;         // New field to track completion status.
    int appointment_duration; // how long appointment will last.
    struct Appointment *next;
    struct Task *task;
} Appointment;

// Task structure
typedef struct Task
{
    char *task_description;           // Dynamic string for task description.
    CompletionLevel completion_level; // Task completed or not.
    bool is_sub_task;                 // Is there a sub-task or not.
    bool is_recurring;                // Is it a recurring task or not.
    bool have_reminder;               // Does it have a reminder for the task.
    unsigned int priority;            // Task priority (1- 5).
    Notes *task_notes;                // Pointer to associated notes.
    Time *task_time;                  // Pointer to associated time (e.g., deadline).
    SubTask *sub_task;                // Sub-task information (optional).
    RecurrenceType recurrence_type;   // Type of recurrence.
    char *reminder_description;       // Reminder description (if any).
    struct Appointment *appointment;  // Associated appointment for the task.
} Task;

typedef struct AppointmentNode
{
    struct Appointment data;
    struct AppointmentNode *next;
} AppointmentNode;

// Node structure
typedef struct Node
{
    Task task;      // storage for task for node.
    Node *head;     // Head of a node.
    Node *next;     // Pointer to the next task.
    Node *previous; // Pointer to the previous task.
    Node *tail;     // tail of a node.
} Node;

// Calendar structure
typedef struct Calendar
{
    Node *tasks;              // Linked list of tasks.
    struct Appointment *head; // Array of appointments.
    struct Appointment *tail;
    Month month; // Month (1 = January, 12 = December).
    int total_days;
    int days_in_current_month;
    int appointment_count; // Number of appointments.
    int year;              // Year (e.g., 2025).
    int day;               // Day of the month.
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

void add_sub_tasks(SubTask *sub_task, Node *new_task)
{
    if (sub_task == NULL && new_task == NULL)
    {
        return;
    }

    SubTask *current = sub_task;

    while (current->next != NULL)
    {
        current = current->next;
    }

    SubTask *new_node = (SubTask *)malloc(sizeof(SubTask));

    if (new_node == NULL)
    {
        return;
    }

    current->next = new_node;
    new_node->previous = current;
    new_node->next = NULL;
}

static const int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static const int leap_days[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int get_days_in_month(Month month, int year)
{

    return is_leap_year(year) ? leap_days[month - 1] : days_in_month[month - 1];
}

int days_in_year(int year)
{
    return 365 + (is_leap_year(year) ? 1 : 0);
}

AppointmentNode *create_appointment_node(Appointment *appointment)
{
    AppointmentNode *new_node = (AppointmentNode *)malloc(sizeof(AppointmentNode));

    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->data = *appointment;
    new_node->next = NULL;
    return new_node;
}

void free_appointment_node(AppointmentNode *node)
{
    if (node != NULL)
    {
        free(node);
    }
}

Task *create_task(const char *task_description, CompletionLevel completion_level, bool is_subtask, bool is_recurring, bool have_reminder,
                  unsigned int priority, RecurrenceType recurrence_type, Time *minutes, Time *hours, const char *notes_description,
                  const char *reminder_description, SubTask *sub_task)
{
    Node *new_task = (Node *)malloc(sizeof(Node));

    if (new_task == NULL)
    {
        return NULL;
    }

    new_task->task.have_reminder = have_reminder;
    new_task->task.is_recurring = is_recurring;
    new_task->task.priority = priority;
    new_task->task.recurrence_type = recurrence_type;
    new_task->task.completion_level = completion_level;

    // Check if hours and minutes are valid before dereferencing
    if (hours != NULL && minutes != NULL)
    {
        new_task->task.task_time->hours = hours->hours;
        new_task->task.task_time->minutes = minutes->minutes;
    }
    else
    {
        // Handle invalid time input
        printf("Error: Invalid time input\n");
        free(new_task);
        return NULL;
    }

    // Creating and getting task description.
    if (task_description != NULL)
    {
        new_task->task.task_description = strdup(task_description);
        if (new_task->task.task_description == NULL)
        {
            printf("Memory allocation failed for task description\n");
            free(new_task);
            return NULL;
        }
    }
    else
    {
        new_task->task.task_description = NULL;
    }

    // Creating and getting notes description.
    if (notes_description != NULL)
    {
        new_task->task.task_notes = (Notes *)malloc(sizeof(Notes));
        if (new_task->task.task_notes == NULL)
        {
            printf("Memory allocation failed for notes\n");
            free(new_task);
            return NULL;
        }
        new_task->task.task_notes->notes_description = strdup(notes_description);
        if (new_task->task.task_notes->notes_description == NULL)
        {
            printf("Memory allocation failed for notes description\n");
            free(new_task->task.task_notes);
            free(new_task);
            return NULL;
        }
    }
    else
    {
        new_task->task.task_notes = NULL;
    }

    // Creating and getting reminder description.
    if (reminder_description != NULL)
    {
        new_task->task.reminder_description = strdup(reminder_description);
        if (new_task->task.reminder_description == NULL)
        {
            printf("Memory allocation failed for reminder description\n");
            free(new_task->task.reminder_description);
            free(new_task);
            return NULL;
        }
    }
    else
    {
        new_task->task.reminder_description = NULL;
    }

    // Creating and getting mutiple sub-task in singly linked list form.
    new_task->task.sub_task = sub_task;

    if (sub_task != NULL && sub_task->next != NULL)
    {
        add_sub_tasks(sub_task, new_task);
    }

    return new_task;
}

void add_task(Task **task, const char *task_description, CompletionLevel completion_level, bool is_subtask, bool is_recurring, bool have_reminder,
              unsigned int priority, RecurrenceType recurrence_type, Time *minutes, Time *hours, const char *notes_description,
              const char *reminder_description, SubTask *sub_task)
{
    if (task == NULL)
    {
        return;
    }

    *task = create_task(task_description, completion_level, is_subtask, is_recurring, have_reminder,
                        priority, recurrence_type, minutes, hours, notes_description,
                        reminder_description, sub_task);

    if (*task == NULL)
    {
        fprintf(stderr, "Failed to create task\n");
    }
}


void free_task(Node *task)
{
    if (task == NULL)
    {
        return;
    }

    // Free task description
    if (task->task.task_description != NULL)
    {
        free(task->task.task_description);
    }

    // free notes decription
    if (task->task.task_notes != NULL)
    {
        free(task->task.task_notes->notes_description);
        free(task->task.task_notes);
    }

    if (task->task.task_time != NULL)
    {
        free(task->task.task_time);
    }

    // Free reminder description
    if (task->task.reminder_description != NULL)
    {
        free(task->task.reminder_description);
    }

    // Free up sub-tasks
    while (task->task.sub_task != NULL)
    {
        SubTask *next = task->task.sub_task->next;
        free(task->task.sub_task);
        task->task.sub_task = next;
    }
}

Calendar *create_calendar(Node *task, int day, int month, int year)
{
    // Input validation
    if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1 || year > MAX_YEAR)
    {
        fprintf(stderr, "Invalid date parameters\n");
        return NULL;
    }

    Calendar *new_calendar = (Calendar *)malloc(sizeof(Calendar));

    if (new_calendar == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Calendar\n");
        return NULL;
    }

    new_calendar->day = day;
    new_calendar->month = (Month)month;
    new_calendar->year = year;
    new_calendar->tasks = task;
    new_calendar->appointment_count = 0;
    new_calendar->head = NULL;
    new_calendar->tail = NULL;

    new_calendar->total_days = days_in_year(year);
    new_calendar->days_in_current_month = get_days_in_month((Month)month, year);

    // Optional appointment creation
    if (task != NULL && task->task.appointment != NULL)
    {
        Appointment *new_appointment = task->task.appointment;
        new_calendar->head = new_appointment;
        new_calendar->tail = new_appointment;
        new_calendar->appointment_count++;
    }

    return new_calendar;
}

void free_appointment(Appointment *appointment)
{
    if (appointment != NULL)
    {
        if (appointment->description != NULL)
        {
            free(appointment->description);
        }
        if (appointment->time != NULL)
        {
            free(appointment->time);
        }

        free(appointment);
    }
}

void free_calendar(Calendar *calendar)
{
    if (calendar == NULL)
    {
        return;
    }

    Appointment *current = calendar->head;

    while (current != NULL)
    {
        Appointment *next = current->next;
        free_appointment(current);
        current = next;
    }

    free(calendar);
}

Appointment *create_appointment(int id, int day, const char *appointment_description, bool is_complete, int appointment_duration, const char *time)
{
    Appointment *new_appointment = (Appointment *)malloc(sizeof(Appointment));

    if (new_appointment == NULL)
    {
        return NULL;
    }

    new_appointment->id = id;
    new_appointment->day = day;
    new_appointment->appointment_duration = appointment_duration;
    new_appointment->is_complete = is_complete;

    if (appointment_description != NULL)
    {
        new_appointment->description = strdup(appointment_description);
        if (new_appointment->description == NULL)
        {
            free(new_appointment);
            return NULL;
        }
    }
    else
    {
        new_appointment->description = NULL;
    }

    if (time != NULL)
    {
        new_appointment->time = strdup(time);
        if (new_appointment->time == NULL)
        {
            free(new_appointment->description);
            free(new_appointment);
            return NULL;
        }
    }
    else
    {
        new_appointment->time = NULL;
    }

    new_appointment->next = NULL;

    return new_appointment;
}

void add_appointment(Calendar *calendar, Appointment *appointment)
{
    if (appointment == NULL)
    {
        return;
    }

    if (calendar->head == NULL)
    {
        calendar->head = calendar->tail = appointment;
    }
    else
    {
        calendar->tail->next = appointment;
        calendar->tail = appointment;
    }

    calendar->appointment_count++;
}

int main()
{
    return 0;
}