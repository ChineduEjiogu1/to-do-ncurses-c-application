#include <ncurses.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Notes structure
typedef struct Notes {
    char *notes_description; // Dynamic string for notes description.
} Notes;

// Time structure
typedef struct Time {
    int minutes; // Minute of the hour (0-59).
    int hours;   // Hour of the day (0-23).
} Time;

// Forward declaration for Node
typedef struct Node Node;

// Task structure
typedef struct Task {
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
struct Node {
    Task task;       // Task stored in this node.
    Node *next;      // Pointer to the next task.
    Node *previous;  // Pointer to the previous task.
};

// Appointment structure
typedef struct Appointment {
    char *time;        // Appointment time
    char *description; // Appointment description
    int day;           // Day of the month for the appointment
} Appointment;

// Calendar structure
typedef struct Calendar {
    Node *tasks;           // Linked list of tasks.
    Appointment *appointments; // Array of appointments.
    int appointment_count; // Number of appointments.
    int year;              // Year (e.g., 2025).
    int month;             // Month (1 = January, 12 = December).
    int day;               // Day of the month.
} Calendar;

// Create a task
Node *create_task(const char *description, unsigned int priority, const char *notes_description) {
    Node *new_node = (Node *)malloc(sizeof(Node));

    if (description) {
        new_node->task.task_description = strdup(description);
    } else {
        new_node->task.task_description = NULL;
    }

    new_node->task.is_complete = false;
    new_node->task.is_sub_task = false;
    new_node->task.is_recurring = false;
    new_node->task.have_reminder = false;
    new_node->task.priority = priority;

    if (notes_description) {
        new_node->task.task_notes = (Notes *)malloc(sizeof(Notes));
        new_node->task.task_notes->notes_description = strdup(notes_description);
    } else {
        new_node->task.task_notes = NULL;
    }

    new_node->task.task_time = NULL;
    new_node->next = new_node->previous = NULL;

    return new_node;
}

// Free a task node
void free_task(Node *node) {
    if (node) {
        free(node->task.task_description);
        if (node->task.task_notes) {
            free(node->task.task_notes->notes_description);
            free(node->task.task_notes);
        }
        free(node);
    }
}

// Display tasks for the day
void display_tasks(WINDOW *tasks_win, Node *tasks) {
    wclear(tasks_win);
    box(tasks_win, 0, 0);
    mvwprintw(tasks_win, 0, 2, " Tasks ");

    int row = 1;
    while (tasks) {
        mvwprintw(tasks_win, row++, 2, "[%c] %s",
                  tasks->task.is_complete ? 'x' : ' ',
                  tasks->task.task_description);
        tasks = tasks->next;
    }

    if (row == 1) {
        mvwprintw(tasks_win, row, 2, "No tasks for this day.");
    }

    wrefresh(tasks_win);
}

// Display appointments for the day
void display_appointments(WINDOW *appointments_win, Appointment *appointments, int appointment_count, int day) {
    wclear(appointments_win);
    box(appointments_win, 0, 0);
    mvwprintw(appointments_win, 0, 2, " Appointments ");

    int row = 1;
    for (int i = 0; i < appointment_count; ++i) {
        if (appointments[i].day == day) {
            mvwprintw(appointments_win, row++, 2, "%s - %s",
                      appointments[i].time, appointments[i].description);
        }
    }

    if (row == 1) {
        mvwprintw(appointments_win, row, 2, "No appointments for this day.");
    }

    wrefresh(appointments_win);
}

// Display calendar
void display_calendar(WINDOW *calendar_win, int highlight_day) {
    wclear(calendar_win);
    box(calendar_win, 0, 0);
    mvwprintw(calendar_win, 0, 2, " Calendar ");

    mvwprintw(calendar_win, 1, 2, "January 2025");
    mvwprintw(calendar_win, 2, 2, "Mon Tue Wed Thu Fri Sat Sun");

    int start_day = 3; // January 1 starts on Wednesday
    int total_days = 31;
    int col = start_day;

    for (int day = 1; day <= total_days; ++day) {
        if (day == highlight_day) {
            wattron(calendar_win, A_REVERSE);
        }
        mvwprintw(calendar_win, 3 + (col / 7), 2 + (col % 7) * 4, "%2d", day);
        if (day == highlight_day) {
            wattroff(calendar_win, A_REVERSE);
        }
        col++;
    }

    wrefresh(calendar_win);
}

int main() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);

    int height, width;
    getmaxyx(stdscr, height, width);

    int tasks_width = width / 2;
    int calendar_width = width - tasks_width;

    WINDOW *tasks_win = newwin(height - 5, tasks_width, 0, 0);
    WINDOW *calendar_win = newwin(height - 5, calendar_width, 0, tasks_width);
    WINDOW *appointments_win = newwin(5, width, height - 5, 0);

    Calendar calendar = {NULL, NULL, 0, 2025, 1, 1};

    // Sample data
    Node *task1 = create_task("Complete project report", 1, "High priority task");
    Node *task2 = create_task("Prepare for meeting", 2, "Medium priority");
    task1->next = task2;
    calendar.tasks = task1;

    Appointment appointments[] = {
        {"10:00", "Doctor's Appointment", 10},
        {"14:00", "Team Meeting", 15}
    };
    calendar.appointments = appointments;
    calendar.appointment_count = 2;

    int highlight_day = 1;

    while (true) {
        display_calendar(calendar_win, highlight_day);
        display_tasks(tasks_win, calendar.tasks);
        display_appointments(appointments_win, calendar.appointments, calendar.appointment_count, highlight_day);

        int ch = getch();
        if (ch == 'q') break;

        switch (ch) {
            case KEY_LEFT: if (highlight_day > 1) highlight_day--; break;
            case KEY_RIGHT: if (highlight_day < 31) highlight_day++; break;
            case KEY_UP: if (highlight_day > 7) highlight_day -= 7; break;
            case KEY_DOWN: if (highlight_day <= 24) highlight_day += 7; break;
        }
    }

    endwin();
    return 0;
}
