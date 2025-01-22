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
    unsigned int priority;           // Task priority (1- 5).
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
    bool is_complete;  // New field to track completion status
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

// Helper function to create a new node
Node *create_node()
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return new_node;
}

// Helper function to free a node
void free_node(Node *node)
{
    if (node == NULL)
    {
        return;
    }
    free(node->task.task_description);
    free(node->task.reminder_description);
    free(node->task.task_time);
    free(node->task.task_notes);
    free(node);
}

// Helper function to create a new time struct
Time *create_time(int hours, int minutes)
{
    Time *new_time = (Time *)malloc(sizeof(Time));
    if (new_time == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_time->hours = hours;
    new_time->minutes = minutes;
    return new_time;
}

// Helper function to create a new notes struct
Notes *create_notes(const char *notes)
{
    Notes *new_notes = (Notes *)malloc(sizeof(Notes));
    if (new_notes == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_notes->notes_description = strdup(notes);
    if (new_notes->notes_description == NULL)
    {
        free(new_notes);
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return new_notes;
}

// Helper function to create a new appointment struct
Appointment *create_appointment()
{
    Appointment *new_appointment = (Appointment *)malloc(sizeof(Appointment));
    if (new_appointment == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    new_appointment->time = NULL;
    new_appointment->description = NULL;
    new_appointment->day = 0;
    return new_appointment;
}

// Function to set appointment details
void set_appointment_details(Appointment *appointment, const char *time, const char *description, int day)
{
    if (appointment == NULL)
    {
        return;
    }
    appointment->time = strdup(time);
    appointment->description = strdup(description);
    appointment->day = day;
}

Node *create_task(const char *description, bool is_complete, bool is_subtask, bool is_recurring, bool have_reminder,
                  unsigned int priority, const char *reminder_description, RecurrenceType recurrence, bool appointment, int hours, int minutes, const char *notes)
{
    // Input validation for task description
    if (description == NULL || strlen(description) == 0 || strlen(description) > 100)
    {
        fprintf(stderr, "Invalid task description. It should be a non-empty string with a maximum length of 100 characters.\n");
        return NULL;
    }

    // Default priority to 5 if not provided
    if (priority == 0)
    {
        priority = 5;
    }
    else if (priority < 1 || priority > 5)
    {
        fprintf(stderr, "Invalid priority. It should be an integer between 1 and 5.\n");
        return NULL;
    }

    // Input validation for hour
    if (hours < 0 || hours > 23)
    {
        fprintf(stderr, "Invalid hour. It should be an integer between 0 and 23.\n");
        return NULL;
    }

    // Input validation for minutes
    if (minutes < 0 || minutes > 59)
    {
        fprintf(stderr, "Invalid minutes. It should be an integer between 0 and 59.\n");
        return NULL;
    }

    // Ensure other parameters are of the correct data type
    if (sizeof(is_complete) != sizeof(bool) || sizeof(is_subtask) != sizeof(bool) || sizeof(is_recurring) != sizeof(bool) || sizeof(have_reminder) != sizeof(bool) || sizeof(appointment) != sizeof(bool))
    {
        fprintf(stderr, "Invalid boolean parameter.\n");
        return NULL;
    }

    if (sizeof(recurrence) != sizeof(RecurrenceType))
    {
        fprintf(stderr, "Invalid recurrence type.\n");
        return NULL;
    }

    Node *new_task = create_node();
    if (new_task == NULL)
    {
        return NULL;
    }

    new_task->task.task_description = strdup(description);
    if (new_task->task.task_description == NULL)
    {
        free_node(new_task);
        return NULL;
    }

    new_task->task.is_complete = is_complete;
    new_task->task.is_sub_task = is_subtask;
    new_task->task.is_recurring = is_recurring;
    new_task->task.recurrence_type = recurrence;
    new_task->task.priority = priority;
    new_task->task.have_reminder = have_reminder;

    if (have_reminder)
    {
        new_task->task.reminder_description = strdup(reminder_description);
        if (new_task->task.reminder_description == NULL)
        {
            free_node(new_task);
            return NULL;
        }
    }
    else
    {
        new_task->task.reminder_description = NULL;
    }

    new_task->task.task_time = create_time(hours, minutes);
    if (new_task->task.task_time == NULL)
    {
        free_node(new_task);
        return NULL;
    }

    if (notes)
    {
        new_task->task.task_notes = create_notes(notes);
        if (new_task->task.task_notes == NULL)
        {
            free_node(new_task);
            return NULL;
        }
    }
    else
    {
        new_task->task.task_notes = NULL;
    }

    new_task->task.sub_task = NULL;
    new_task->task.appointment = appointment ? create_appointment() : NULL;
    if (new_task->task.appointment != NULL && new_task->task.appointment == NULL)
    {
        free_node(new_task);
        return NULL;
    }
    new_task->next = new_task->previous = NULL;

    return new_task;
}

void create_sub_task(Node *task, const char *description)
{
    // Check if task is NULL
    if (task == NULL || description == NULL)
    {
        return;
    }

    // Create a new SubTask struct
    SubTask *new_sub_task = (SubTask *)malloc(sizeof(SubTask));

    // Check if memory allocation was successful
    if (new_sub_task == NULL)
    {
        return;
    }

    // Copy the description into the new SubTask struct
    new_sub_task->sub_task_description = strdup(description);

    // Check if memory allocation for the description was successful
    if (new_sub_task->sub_task_description == NULL)
    {
        free(new_sub_task);
        return;
    }

    // Initialize the next pointer to NULL
    new_sub_task->next = NULL;

    // If the task already has a sub-task, append the new sub-task to the end
    if (task->task.sub_task != NULL)
    {
        SubTask *current = task->task.sub_task;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_sub_task;
    }
    else
    {
        // Otherwise, set the new sub-task as the first sub-task
        task->task.sub_task = new_sub_task;
    }
}

void assign_notes_to_task(Node *task, const char *description)
{
    // Check if task is NULL to prevent segmentation faults
    if (task == NULL)
    {
        return;
    }

    // Create a new Notes struct
    Notes *new_notes = (Notes *)malloc(sizeof(Notes));

    // Check if memory allocation was successful
    if (new_notes == NULL)
    {
        return;
    }

    // Copy the description into the new Notes struct
    new_notes->notes_description = strdup(description);

    // Check if memory allocation for the description was successful
    if (new_notes->notes_description == NULL)
    {
        free(new_notes);
        return;
    }

    // Assign the new Notes struct to the task
    task->task.task_notes = new_notes;
}

// Update task function
void update_task(Node *task, const char *description, bool is_complete, bool is_subtask, bool is_recurring, bool have_reminder,
                 unsigned int priority, const char *reminder_description, RecurrenceType recurrence, bool appointment, int hours, int minutes, const char *notes)
{
    if (task == NULL)
    {
        return;
    }

    // Update task description
    if (description != NULL)
    {
        free(task->task.task_description);
        task->task.task_description = strdup(description);
        if (task->task.task_description == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Update task fields
    task->task.is_complete = is_complete;
    task->task.is_sub_task = is_subtask;
    task->task.is_recurring = is_recurring;
    task->task.have_reminder = have_reminder;
    task->task.priority = priority;
    task->task.recurrence_type = recurrence;

    // Update reminder description
    if (have_reminder && reminder_description != NULL)
    {
        free(task->task.reminder_description);
        task->task.reminder_description = strdup(reminder_description);
        if (task->task.reminder_description == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
    }

    // Update appointment
    if (appointment)
    {
        // Create a new appointment
        Appointment *new_appointment = create_appointment();
        if (new_appointment == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        task->task.appointment = new_appointment;
    }

    // Update task time
    if (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59)
    {
        Time *new_time = create_time(hours, minutes);
        if (new_time == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        free(task->task.task_time);
        task->task.task_time = new_time;
    }

    // Update task notes
    if (notes != NULL)
    {
        Notes *new_notes = create_notes(notes);
        if (new_notes == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        free(task->task.task_notes);
        task->task.task_notes = new_notes;
    }
}

// Delete task function
void delete_task(Node **head, Node *task)
{
    if (head == NULL || task == NULL)
    {
        return;
    }

    // Check if the task is the head node
    if (*head == task)
    {
        *head = task->next;
        if (*head != NULL)
        {
            (*head)->previous = NULL;
        }
    }
    else
    {
        // Find the task in the linked list
        Node *current = *head;
        while (current != NULL && current->next != task)
        {
            current = current->next;
        }
        if (current != NULL)
        {
            current->next = task->next;
            if (task->next != NULL)
            {
                task->next->previous = current;
            }
        }
    }

    // Free the task's memory
    free(task->task.task_description);
    free(task->task.reminder_description);
    free(task->task.task_time);
    free(task->task.task_notes);
    free(task);
}

Calendar *create_calendar(int year, int month, int day)
{
    Calendar *calendar = (Calendar *)malloc(sizeof(Calendar));
    if (calendar == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    calendar->year = year;
    calendar->month = month;
    calendar->day = day;
    calendar->tasks = NULL;
    calendar->appointments = NULL;
    calendar->appointment_count = 0;
    return calendar;
}

void add_task_to_calendar(Calendar *calendar, Node *task)
{
    if (calendar == NULL || task == NULL)
    {
        return;
    }
    task->next = calendar->tasks;
    if (calendar->tasks != NULL)
    {
        calendar->tasks->previous = task;
    }
    calendar->tasks = task;
    task->previous = NULL;
}

void create_appointment_for_task(Node *task, const char *time, const char *description, int day)
{
    if (task == NULL)
    {
        return;
    }
    task->task.appointment = (Appointment *)malloc(sizeof(Appointment));
    if (task->task.appointment == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    task->task.appointment->time = strdup(time);
    task->task.appointment->description = strdup(description);
    task->task.appointment->day = day;
}

// Add appointment to calendar function
void add_appointment_to_calendar(Calendar *calendar, Appointment *appointment)
{
    if (calendar == NULL || appointment == NULL)
    {
        return;
    }

    // Check if the appointment array needs to be resized
    if (calendar->appointment_count >= 10)
    {
        // Resize the appointment array
        Appointment *new_appointments = (Appointment *)realloc(calendar->appointments, (calendar->appointment_count + 10) * sizeof(Appointment));
        if (new_appointments == NULL)
        {
            fprintf(stderr, "Memory reallocation failed\n");
            exit(EXIT_FAILURE);
        }
        calendar->appointments = new_appointments;
    }

    // Add the appointment to the calendar
    calendar->appointments[calendar->appointment_count] = *appointment;
    calendar->appointment_count++;

    printw("Appointment added to calendar successfully\n");
}

Appointment *create_appointment(const char *time, const char *description, int day)
{
    Appointment *appointment = (Appointment *)malloc(sizeof(Appointment));
    if (appointment == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    appointment->time = strdup(time);
    if (appointment->time == NULL)
    {
        fprintf(stderr, "Memory allocation failed for time string\n");
        free(appointment);
        exit(EXIT_FAILURE);
    }

    appointment->description = strdup(description);
    if (appointment->description == NULL)
    {
        fprintf(stderr, "Memory allocation failed for description string\n");
        free(appointment->time);
        free(appointment);
        exit(EXIT_FAILURE);
    }

    appointment->day = day;
    appointment->is_complete = false;

    return appointment;
}

void update_appointment(Appointment *appointment, const char *new_time, const char *new_description, int new_day)
{
    if (appointment == NULL)
    {
        fprintf(stderr, "Appointment pointer is NULL\n");
        return;
    }

    // Update time
    if (new_time != NULL)
    {
        free(appointment->time);
        appointment->time = strdup(new_time);
        if (appointment->time == NULL)
        {
            fprintf(stderr, "Memory allocation failed for new time string\n");
        }
    }

    // Update description
    if (new_description != NULL)
    {
        free(appointment->description);
        appointment->description = strdup(new_description);
        if (appointment->description == NULL)
        {
            fprintf(stderr, "Memory allocation failed for new description string\n");
        }
    }

    // Update day
    if (new_day != -1) // Use -1 as a sentinel value to indicate no update
    {
        appointment->day = new_day;
    }
}

void delete_appointment(Appointment *appointment)
{
    if (appointment == NULL)
    {
        fprintf(stderr, "Appointment pointer is NULL\n");
        return;
    }

    // Free the time and description strings
    free(appointment->time);
    free(appointment->description);

    // Free the Appointment struct itself
    free(appointment);
}

void mark_task_as_complete(Task *task)
{
    if (task == NULL)
    {
        fprintf(stderr, "Task pointer is NULL\n");
        return;
    }

    task->is_complete = true;
}

void mark_appointment_as_complete(Appointment *appointment)
{
    if (appointment == NULL)
    {
        fprintf(stderr, "Appointment pointer is NULL\n");
        return;
    }

    appointment->is_complete = true;
}

#include <ncurses.h>

// Function to get user input using ncurses
void get_user_input(Task *task, Appointment *appointment)
{
    initscr();              // Initialize ncurses
    noecho();               // Don't echo keyboard input
    cbreak();               // Enable cbreak mode

    // Get task description
    echo();
    mvprintw(1, 1, "Enter task description: ");
    refresh();
    getstr(task->task_description);

    // Get task priority
    mvprintw(2, 1, "Enter task priority (1-5): ");
    refresh();
    scanw("%d", &task->priority);

    // Get task completion status
    mvprintw(3, 1, "Is task complete? (y/n): ");
    refresh();
    char complete;
    scanw(" %c", &complete);
    task->is_complete = (complete == 'y');

    // Get sub-task status
    mvprintw(4, 1, "Is there a sub-task? (y/n): ");
    refresh();
    char sub_task;
    scanw(" %c", &sub_task);
    task->is_sub_task = (sub_task == 'y');

    // Get recurring status
    mvprintw(5, 1, "Is task recurring? (y/n): ");
    refresh();
    char recurring;
    scanw(" %c", &recurring);
    task->is_recurring = (recurring == 'y');

    // Get reminder status
    mvprintw(6, 1, "Does task have reminder? (y/n): ");
    refresh();
    char reminder;
    scanw(" %c", &reminder);
    task->have_reminder = (reminder == 'y');

    // Get appointment time
    mvprintw(7, 1, "Enter appointment time (HH:MM AM/PM): ");
    refresh();
    getstr(appointment->time);

    // Get appointment description
    mvprintw(8, 1, "Enter appointment description: ");
    refresh();
    getstr(appointment->description);

    // Get appointment day
    mvprintw(9, 1, "Enter appointment day: ");
    refresh();
    scanw("%d", &appointment->day);

    // Get recurrence type
    mvprintw(10, 1, "Enter recurrence type (0-6): ");
    refresh();
    int recurrence_type;
    scanw("%d", &recurrence_type);
    task->recurrence_type = (RecurrenceType) recurrence_type;

    // Get reminder description
    mvprintw(11, 1, "Enter reminder description: ");
    refresh();
    getstr(task->reminder_description);

    noecho();               // Don't echo keyboard input
    endwin();               // End ncurses mode
}

int main()
{
    // Create a new calendar
    Calendar calendar;
    calendar.tasks = NULL;
    calendar.appointments = NULL;
    calendar.appointment_count = 0;
    calendar.year = 2025;
    calendar.month = 1;
    calendar.day = 1;

    // Create a new task
    Task *task = (Task *)malloc(sizeof(Task));
    task->task_description = (char *)malloc(100 * sizeof(char));
    task->is_complete = false;
    task->is_sub_task = false;
    task->is_recurring = false;
    task->have_reminder = false;
    task->priority = 0;
    task->task_notes = NULL;
    task->task_time = NULL;
    task->sub_task = NULL;
    task->recurrence_type = NONE;
    task->reminder_description = (char *)malloc(100 * sizeof(char));
    task->appointment = NULL;

    // Create a new appointment
    Appointment *appointment = (Appointment *)malloc(sizeof(Appointment));
    appointment->time = (char *)malloc(100 * sizeof(char));
    appointment->description = (char *)malloc(100 * sizeof(char));
    appointment->day = 0;
    appointment->is_complete = false;

    // Get user input
    get_user_input(task, appointment);

    // Add the appointment to the task
    task->appointment = appointment;

    // Create a new node for the task
    Node *node = (Node *)malloc(sizeof(Node));
    node->task = *task; // Assign the task to the node
    node->next = NULL;
    node->previous = NULL;

    // Add the node to the calendar
    calendar.tasks = node;

    // Print the calendar
    printf("Calendar for %d-%d-%d:\n", calendar.year, calendar.month, calendar.day);
    printf("Tasks:\n");
    printf("  - %s (Priority: %d)\n", task->task_description, task->priority);
    printf("Appointments:\n");
    printf("  - %s on day %d at %s\n", task->appointment->description, task->appointment->day, task->appointment->time);

    // Mark the task as complete
    task->is_complete = true;
    printf("Task marked as complete\n");

    // Mark the appointment as complete
    task->appointment->is_complete = true;
    printf("Appointment marked as complete\n");

    // Free allocated memory
    free(task->appointment->time);
    free(task->appointment->description);
    free(task->appointment);
    free(task->task_description);
    free(task->reminder_description);
    free(task);
    free(node);

    return EXIT_SUCCESS;
}