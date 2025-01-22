#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// Enum for recurrence types
typedef enum
{
    NONE,
    DAILY,
    WEEKLY,
    MONTHLY,
    YEARLY
} RecurrenceType;

// Sub-task structure
typedef struct SubTask
{
    char *description; // Sub-task description
} SubTask;

// Task structure
typedef struct Task
{
    char *description;              // Task description
    bool is_sub_task;               // Whether the task has sub-tasks
    SubTask *sub_task;              // Sub-task information (optional)
    bool is_recurring;              // Whether the task is recurring
    RecurrenceType recurrence_type; // Type of recurrence
    bool have_reminder;             // Whether the task has a reminder
    char *reminder_time;            // Reminder time in HH:MM format
    unsigned int priority;          // Task priority
    char *note;                     // Optional note for the task
    char *time;                     // Time in HH:MM format
} Task;

// Prompt user for input and return a dynamically allocated string
char *prompt_user(WINDOW *win, int y, int x, const char *prompt, bool is_required)
{
    char *input = NULL;

    while (true)
    {
        mvwprintw(win, y, x, "%s", prompt);
        wrefresh(win);

        char temp_input[256];
        echo();
        mvwgetnstr(win, y + 1, x, temp_input, sizeof(temp_input) - 1);
        noecho();

        // If input is required and the user enters an empty string, show an error and prompt again
        if (is_required && strlen(temp_input) == 0)
        {
            mvwprintw(win, y + 2, x, "Description cannot be empty! Please enter a valid description.");
            wrefresh(win);
            continue; // Prompt again
        }

        // Allocate memory for the input and return it
        input = strdup(temp_input);
        break;
    }

    return input;
}

// Prompt user for yes/no input
bool prompt_user_bool(WINDOW *win, int y, int x, const char *prompt)
{
    mvwprintw(win, y, x, "%s (y/n):", prompt);
    wrefresh(win);

    char ch;
    while (true)
    {
        ch = wgetch(win);
        if (ch == 'y' || ch == 'Y')
            return true;
        if (ch == 'n' || ch == 'N')
            return false;
    }
}

// Prompt user for an integer input
unsigned int prompt_user_int(WINDOW *win, int y, int x, const char *prompt)
{
    mvwprintw(win, y, x, "%s", prompt);
    wrefresh(win);

    char input[10];
    echo();
    mvwgetnstr(win, y + 1, x, input, sizeof(input) - 1);
    noecho();

    return (unsigned int)atoi(input);
}

// Prompt user for recurrence type
RecurrenceType prompt_recurrence_type(WINDOW *win, int y, int x)
{
    const char *options[] = {"None", "Daily", "Weekly", "Monthly", "Yearly"};
    int choice = 0;
    int key;

    // Enable keypad for arrow key input
    keypad(win, TRUE);

    while (true)
    {
        // Display menu options
        for (int i = 0; i < 5; i++)
        {
            if (i == choice)
            {
                wattron(win, A_REVERSE); // Highlight the selected option
            }
            mvwprintw(win, y + i, x, "%s", options[i]);
            wattroff(win, A_REVERSE);
        }
        wrefresh(win);

        // Capture user input
        key = wgetch(win);
        if (key == KEY_UP)
        {
            choice = (choice == 0) ? 4 : choice - 1; // Wrap around to the last option
        }
        else if (key == KEY_DOWN)
        {
            choice = (choice == 4) ? 0 : choice + 1; // Wrap around to the first option
        }
        else if (key == '\n')
        {
            return (RecurrenceType)choice; // Return the selected option
        }
    }
}

// Gather sub-task details
SubTask *create_sub_task_ui(WINDOW *win)
{
    SubTask *sub_task = (SubTask *)malloc(sizeof(SubTask));
    sub_task->description = prompt_user(win, 16, 2, "Enter sub-task description:", true);
    return sub_task;
}

// Gather task details from the user with description validation
Task *create_task_ui(WINDOW *win)
{
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Create Task ");
    wrefresh(win);

    Task *task = (Task *)malloc(sizeof(Task));
    memset(task, 0, sizeof(Task));

    // Ensure that task description is not empty by using prompt_user with validation
    task->description = prompt_user(win, 1, 2, "Enter task description:", true);

    task->is_sub_task = prompt_user_bool(win, 4, 2, "Does this task have sub-tasks?");
    if (task->is_sub_task)
    {
        task->sub_task = create_sub_task_ui(win);
    }
    task->is_recurring = prompt_user_bool(win, 6, 2, "Is this task recurring?");
    if (task->is_recurring)
    {
        mvwprintw(win, 8, 2, "Select recurrence type:");
        task->recurrence_type = prompt_recurrence_type(win, 9, 4);
    }
    else
    {
        task->recurrence_type = NONE;
    }
    task->have_reminder = prompt_user_bool(win, 14, 2, "Do you want to set a reminder?");
    if (task->have_reminder)
    {
        task->reminder_time = prompt_user(win, 16, 2, "Enter reminder time in HH:MM format:", false);
    }
    task->priority = prompt_user_int(win, 18, 2, "Set priority (1-5):");
    task->note = prompt_user(win, 20, 2, "Enter a note (optional):", false);
    task->time = prompt_user(win, 22, 2, "Enter time in HH:MM format:", false);

    return task;
}

// Display recurrence type as a string
const char *recurrence_to_string(RecurrenceType type)
{
    switch (type)
    {
    case DAILY:
        return "Daily";
    case WEEKLY:
        return "Weekly";
    case MONTHLY:
        return "Monthly";
    case YEARLY:
        return "Yearly";
    default:
        return "None";
    }
}

// Display task details
void display_task(WINDOW *win, Task *task)
{
    werase(win);
    box(win, 0, 0);
    mvwprintw(win, 0, 2, " Task Details ");

    // Displaying task description
    mvwprintw(win, 1, 2, "Description: %s", task->description);

    // Displaying if the task has sub-tasks
    mvwprintw(win, 2, 2, "Sub-task: %s", task->is_sub_task ? "Yes" : "No");

    // If task has sub-tasks, display sub-task description
    if (task->is_sub_task && task->sub_task)
    {
        mvwprintw(win, 3, 4, "Sub-task Description: %s", task->sub_task->description);
    }

    // Displaying if the task is recurring
    mvwprintw(win, 4, 2, "Recurring: %s", task->is_recurring ? "Yes" : "No");

    // If task is recurring, display the recurrence type
    if (task->is_recurring)
    {
        mvwprintw(win, 5, 4, "Recurrence Type: %s", recurrence_to_string(task->recurrence_type));
    }

    // Display reminder status
    mvwprintw(win, 6, 2, "Reminder: %s", task->have_reminder ? "Yes" : "No");

    // If task has reminder, display the reminder time
    if (task->have_reminder)
    {
        mvwprintw(win, 7, 4, "Reminder Time: %s", task->reminder_time);
    }

    // Display priority level
    mvwprintw(win, 8, 2, "Priority: %u", task->priority);

    // Display the note (if any)
    mvwprintw(win, 9, 2, "Note: %s", task->note ? task->note : "None");

    // Display the time (if any)
    mvwprintw(win, 10, 2, "Time: %s", task->time ? task->time : "None");

    // Show message for user to press any key to return
    mvwprintw(win, 12, 2, "Press any key to return to the menu.");

    wrefresh(win);
    wgetch(win);
}

// Free sub-task memory
void free_sub_task(SubTask *sub_task)
{
    if (sub_task)
    {
        free(sub_task->description);
        free(sub_task);
    }
}

// Free task memory
void free_task(Task *task)
{
    if (task)
    {
        free(task->description);
        if (task->is_sub_task)
        {
            free_sub_task(task->sub_task);
        }
        free(task->reminder_time);
        free(task->note);
        free(task->time);
        free(task);
    }
}

int main()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);

    int height, width;
    getmaxyx(stdscr, height, width);

    WINDOW *menu_win = newwin(height, width, 0, 0);
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 0, 2, " Task Manager ");
    wrefresh(menu_win);

    Task **tasks = NULL;
    int task_count = 0;

    while (true)
    {
        werase(menu_win);
        box(menu_win, 0, 0);
        mvwprintw(menu_win, 0, 2, " Task Manager ");
        mvwprintw(menu_win, 2, 2, "1. Create Task");
        mvwprintw(menu_win, 3, 2, "2. View Tasks");
        mvwprintw(menu_win, 4, 2, "3. Quit");
        mvwprintw(menu_win, 6, 2, "Select an option:");
        wrefresh(menu_win);

        int choice = wgetch(menu_win);
        if (choice == '1')
        {
            Task *new_task = create_task_ui(menu_win);
            task_count++;
            tasks = (Task **)realloc(tasks, task_count * sizeof(Task *));
            tasks[task_count - 1] = new_task;
        }
        if (choice == '2' && task_count > 0)
        {
            int selected_task = -1;

            // Display the list of tasks
            werase(menu_win);
            box(menu_win, 0, 0);
            mvwprintw(menu_win, 0, 2, " View Tasks ");
            for (int i = 0; i < task_count; i++)
            {
                mvwprintw(menu_win, 2 + i, 2, "%d. %s", i + 1, tasks[i]->description);
            }
            mvwprintw(menu_win, task_count + 3, 2, "Select a task number and press Enter to view details.");
            wrefresh(menu_win);

            // Wait for the user to select a task by number
            char input[10];
            mvwgetnstr(menu_win, task_count + 4, 2, input, sizeof(input) - 1);
            selected_task = atoi(input) - 1;

            // Check if the selected task number is valid
            if (selected_task >= 0 && selected_task < task_count)
            {
                // Now display the details of the selected task
                werase(menu_win);
                box(menu_win, 0, 0);
                mvwprintw(menu_win, 0, 2, " Task Details: %s ", tasks[selected_task]->description);

                // Display sub-task details if available
                if (tasks[selected_task]->is_sub_task && tasks[selected_task]->sub_task)
                {
                    mvwprintw(menu_win, 2, 4, "Sub-task: %s", tasks[selected_task]->sub_task->description);
                }

                // Display recurring status and recurrence type
                if (tasks[selected_task]->is_recurring)
                {
                    mvwprintw(menu_win, 3, 4, "Recurrence Type: %s", recurrence_to_string(tasks[selected_task]->recurrence_type));
                }

                // Display reminder details if set
                if (tasks[selected_task]->have_reminder)
                {
                    mvwprintw(menu_win, 4, 4, "Reminder Time: %s", tasks[selected_task]->reminder_time);
                }

                // Display priority
                mvwprintw(menu_win, 5, 4, "Priority: %u", tasks[selected_task]->priority);

                // Display notes (if any)
                mvwprintw(menu_win, 6, 4, "Note: %s", tasks[selected_task]->note ? tasks[selected_task]->note : "None");

                // Display time (if any)
                mvwprintw(menu_win, 7, 4, "Time: %s", tasks[selected_task]->time ? tasks[selected_task]->time : "None");

                mvwprintw(menu_win, 9, 2, "Press any key to return to the menu.");
                wrefresh(menu_win);
                wgetch(menu_win); // Wait for a key press before returning to the menu
            }
            else
            {
                mvwprintw(menu_win, task_count + 6, 2, "Invalid task number. Press any key to try again.");
                wrefresh(menu_win);
                wgetch(menu_win);
            }
        }

        else if (choice == '3')
        {
            break;
        }
    }

    // Clean up before exiting
    for (int i = 0; i < task_count; i++)
    {
        free_task(tasks[i]);
    }
    free(tasks);

    endwin();
    return 0;
}
