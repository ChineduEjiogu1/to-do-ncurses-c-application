#include <ncurses/ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_YEAR 9999
#define MAX_TASK 1000
#define MAX_SUB_TASK 1000

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

// Enum for months of a year.
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
    unsigned int priority; // Added priority field
} Appointment;

// Task structure
typedef struct Task
{
    int task_id;                      // Id for task.
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

// Function to add a subtask.
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

// function to calculate leap year.
bool is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Function to get days in a month, most getting the proper leap year month or not.
int get_days_in_month(Month month, int year)
{
    return is_leap_year(year) ? leap_days[month - 1] : days_in_month[month - 1];
}

// Function to get the days in a year.
int days_in_year(int year)
{
    return 365 + (is_leap_year(year) ? 1 : 0);
}

bool is_valid_date(int day, int month, int year)
{
    if (year < 1 || year > 9999)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (day < 1 || day > 31)
        return false;

    if (month == 2)
    {
        if (is_leap_year(year))
            return day <= 29;
        return day <= 28;
    }

    if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return day <= 30;
    }

    return true;
}

// Function to create appointment node.
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

// Function to free appointment node.
void free_appointment_node(AppointmentNode *node)
{
    if (node != NULL)
    {
        free(node);
    }
}

// Fuction to allocate string, helper function.
char *allocate_string(const char *string)
{
    if (string == NULL)
    {
        return NULL;
    }

    char *allocated_string = strdup(string);

    if (allocated_string == NULL)
    {
        fprintf(stderr, "Memory allocation failed for string");
        exit(EXIT_FAILURE);
    }

    return allocated_string;
}

// Function to allocate time, helper function.
Time *allocate_time(Time *source)
{
    if (source == NULL)
    {
        return NULL;
    }

    Time *allocated_time = (Time *)malloc(sizeof(Time));

    if (allocated_time == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Time\n");
        exit(EXIT_FAILURE);
    }

    allocated_time->hours = source->hours;
    allocated_time->minutes = source->minutes;
    return allocated_time;
}

// Function to free allocated time.
void free_time(Time *time_ptr)
{
    if (time_ptr != NULL)
    {
        free(time_ptr);
        time_ptr = NULL;
    }
}

// Function to allocate notes, helper function.
Notes *allocate_notes(const char *description)
{
    if (description == NULL)
    {
        return NULL;
    }

    Notes *allocated_notes = (Notes *)malloc(sizeof(Notes));

    if (allocated_notes == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Notes\n");
        exit(EXIT_FAILURE);
    }

    allocated_notes->notes_description = allocate_string(description);

    return allocated_notes;
}

// Function to create task.
Task *create_task(const char *task_description, CompletionLevel completion_level, bool is_subtask, bool is_recurring, bool have_reminder,
                  unsigned int priority, RecurrenceType recurrence_type, Time *minutes, Time *hours, const char *notes_description,
                  const char *reminder_description, SubTask *sub_task)
{
    Task *new_task = (Task *)malloc(sizeof(Task));

    if (new_task == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Task\n");
        return NULL;
    }

    new_task->have_reminder = have_reminder;
    new_task->is_recurring = is_recurring;
    new_task->priority = priority;
    new_task->recurrence_type = recurrence_type;
    new_task->completion_level = completion_level;
    new_task->sub_task = sub_task;

    new_task->task_time = allocate_time(hours);
    new_task->task_description = allocate_string(task_description);
    new_task->task_notes = allocate_notes(notes_description);
    new_task->reminder_description = allocate_string(reminder_description);

    return new_task;
}

// Function to free task if deleted.
void free_task(Task *task)
{
    if (!task)
    {
        return;
    }

    // Free task description
    free(task->task_description);
    if (task->task_notes)
    {
        free(task->task_notes->notes_description);
        free(task->task_notes);
    }

    free(task->task_time);
    free(task->reminder_description);

    // Free up sub-tasks
    while (task->sub_task)
    {
        SubTask *next = task->sub_task->next;
        free(task->sub_task);
        task->sub_task = next;
    }

    // Free the task itself
    free(task);
}

// Function to add task.
bool add_task(Calendar *calendar, const char *task_description, CompletionLevel completion_level, bool is_subtask, bool is_recurring, bool have_reminder,
              unsigned int priority, RecurrenceType recurrence_type, Time *minutes, Time *hours, const char *notes_description,
              const char *reminder_description, SubTask *sub_task)
{
    if (!calendar)
        return false;

    Task *new_task = create_task(task_description, completion_level, is_subtask, is_recurring, have_reminder,
                                 priority, recurrence_type, minutes, hours, notes_description,
                                 reminder_description, sub_task);

    if (!new_task)
    {
        fprintf(stderr, "Failed to create task\n");
        return false;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node)
    {
        free_task(new_task);
        fprintf(stderr, "Memory allocation failed for new node\n");
        return false;
    }

    new_node->task = *new_task;
    new_node->next = NULL;
    new_node->previous = calendar->tasks ? calendar->tasks->tail : NULL;

    if (!calendar->tasks)
    {
        calendar->tasks = new_node;
        calendar->tasks->head = calendar->tasks;
        calendar->tasks->tail = calendar->tasks;
    }
    else
    {
        calendar->tasks->tail->next = new_node;
        calendar->tasks->tail = new_node;
    }

    free(new_task);
    return true;
}

// Function to update task.
Task *update_task(const char *task_description, CompletionLevel completion_level, bool is_subtask, bool is_recurring, bool have_reminder,
                  unsigned int priority, RecurrenceType recurrence_type, Time *minutes, Time *hours, const char *notes_description,
                  const char *reminder_description, SubTask *sub_task)
{
    Task *update_task = (Task *)malloc(sizeof(Task));
    if (update_task == NULL)
    {
        fprintf(stderr, "Memory allocation failed for Task\n");
        return NULL;
    }

    update_task->have_reminder = have_reminder;
    update_task->is_recurring = is_recurring;
    update_task->priority = priority;
    update_task->recurrence_type = recurrence_type;
    update_task->completion_level = completion_level;
    update_task->sub_task = sub_task;

    update_task->task_time = allocate_time(hours);
    update_task->task_description = allocate_string(task_description);
    update_task->task_notes = allocate_notes(notes_description);
    update_task->reminder_description = allocate_string(reminder_description);

    return update_task;
}

void delete_task(Node **task_list, int task_id)
{
    if (*task_list == NULL)
    {
        fprintf(stderr, "Error: Empty task list\n");
        return;
    }

    if ((*task_list)->task.task_id == task_id)
    {
        Node *temp = *task_list;
        *task_list = (*task_list)->next;
        free_task(&temp->task); // Pass the address of the task
        free(temp);             // Free the Node structure
        return;
    }

    Node *current = *task_list;
    while (current->next != NULL)
    {
        if (current->next->task.task_id == task_id)
        {
            Node *temp = current->next;
            current->next = current->next->next;
            free_task(&temp->task); // Pass the address of the task
            free(temp);             // Free the Node structure
            return;
        }
        current = current->next;
    }

    fprintf(stderr, "Task with ID %d not found\n", task_id);
}

// Function to add appointment.
void add_appointment(Calendar *calendar, Appointment *new_appointment)
{
    if (new_appointment == NULL)
        return;

    if (calendar->head == NULL || new_appointment->priority > calendar->head->priority)
    {
        new_appointment->next = calendar->head;
        calendar->head = new_appointment;
        if (calendar->tail == NULL)
            calendar->tail = new_appointment;
    }
    else
    {
        Appointment *current = calendar->head;
        while (current->next != NULL && new_appointment->priority <= current->next->priority)
        {
            current = current->next;
        }
        new_appointment->next = current->next;
        current->next = new_appointment;
        if (new_appointment->next == NULL)
            calendar->tail = new_appointment;
    }

    calendar->appointment_count++;
}

// Function to create calendar.
Calendar *create_calendar(Node *task, int day, int month, int year)
{
    if (!is_valid_date(day, month, year))
    {
        return NULL;
    }

    Calendar *calendar = (Calendar *)calloc(1, sizeof(Calendar));
    if (!calendar)
    {
        return NULL;
    }

    calendar->day = day;
    calendar->month = (Month)month;
    calendar->year = year;
    calendar->tasks = task;
    calendar->total_days = days_in_year(year);
    calendar->days_in_current_month = get_days_in_month((Month)month, year);

    if (task && task->task.appointment)
    {
        add_appointment(calendar, task->task.appointment);
    }

    return calendar;
}

// Function to allocated free appointment.
void free_appointment(Appointment *appointment)
{
    if (!appointment)
        return;

    free(appointment->description);
    free(appointment->time);

    free(appointment);
}

// Function to mark task as different completion level.
void mark_task(Task *task, CompletionLevel completion_level)
{
    if (task == NULL)
    {
        fprintf(stderr, "Error: Null task pointer\n");
        return;
    }

    task->completion_level = completion_level;
}

// Function to free allcoated Calendar.
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

// Comparison function to determine the correct position for the new appointment
int compare_appointments(Appointment *a, Appointment *b)
{
    if (a->priority > b->priority)
        return -1;
    if (a->priority < b->priority)
        return 1;
    return 0;
}

// Function to create appointment.
Appointment *create_appointment(int id, int day, const char *appointment_description, bool is_complete, int appointment_duration, const char *time, unsigned int priority)
{
    Appointment *new_appointment = (Appointment *)malloc(sizeof(Appointment));

    if (new_appointment == NULL)
    {
        fprintf(stderr, "Memory allocation failed for appointment");
        return NULL;
    }

    new_appointment->id = id;
    new_appointment->day = day;
    new_appointment->appointment_duration = appointment_duration;
    new_appointment->is_complete = is_complete;
    new_appointment->description = allocate_string(appointment_description);
    new_appointment->time = allocate_string(time);
    new_appointment->next = NULL;
    new_appointment->priority = priority; // Set the priority

    return new_appointment;
}

// Function to update appointment.
Appointment *update_appointment(Appointment *appointment, int id, int day, const char *appointment_description, bool is_complete, int appointment_duration, const char *time)
{
    if (appointment == NULL)
    {
        fprintf(stderr, "Error: Null appointment pointer\n");
        return NULL;
    }

    appointment->id = id;
    appointment->day = day;
    appointment->appointment_duration = appointment_duration;
    appointment->is_complete = is_complete;

    free(appointment->description);
    appointment->description = allocate_string(appointment_description);

    free(appointment->time);
    appointment->time = allocate_string(time);

    return appointment;
}

int main()
{

    return 0;
}