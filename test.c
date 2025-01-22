#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct Task
{
    char *description;
    bool is_complete;
    struct Task *next;
} Task;

void init_ui()
{
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    keypad(stdscr, TRUE);
}

void end_ui()
{
    endwin();
}

void display_tasks(WINDOW *win, Task *tasks, int selected_task)
{
    werase(win);
    box(win, 0, 0);
    int y = 1;
    Task *current = tasks;
    int i = 0;
    while (current != NULL)
    {
        if (i == selected_task)
            wattron(win, A_REVERSE);
        mvwprintw(win, y++, 2, "[%c] %s", current->is_complete ? 'x' : ' ', current->description);
        wattroff(win, A_REVERSE);
        current = current->next;
        i++;
    }
    wrefresh(win);
}

void add_task(Task **tasks, const char *description)
{
    Task *new_task = (Task *)malloc(sizeof(Task));
    new_task->description = strdup(description);
    new_task->is_complete = false;
    new_task->next = *tasks;
    *tasks = new_task;
}

void mark_task_complete(Task *tasks, int index)
{
    Task *current = tasks;
    int i = 0;
    while (current != NULL && i < index)
    {
        current = current->next;
        i++;
    }
    if (current != NULL)
    {
        current->is_complete = !current->is_complete;
    }
}

void delete_task(Task **tasks, int index)
{
    Task *current = *tasks;
    Task *previous = NULL;
    int i = 0;
    while (current != NULL && i < index)
    {
        previous = current;
        current = current->next;
        i++;
    }
    if (current != NULL)
    {
        if (previous == NULL)
        {
            *tasks = current->next;
        }
        else
        {
            previous->next = current->next;
        }
        free(current->description);
        free(current);
    }
}

int main()
{
    Task *tasks = NULL;
    char input[100];
    int ch;
    int selected_task = 0;
    int task_count = 0;

    init_ui();

    WINDOW *win = newwin(LINES - 2, COLS - 2, 1, 1);
    box(win, 0, 0);

    while (true)
    {
        display_tasks(win, tasks, selected_task);
        mvprintw(LINES - 1, 0, "Commands: [a]dd task, [t]oggle complete, [d]elete task, [q]uit");
        refresh();
        ch = getch();
        if (ch == 'q')
        {
            break;
        }
        else if (ch == 'a')
        {
            mvprintw(LINES - 1, 0, "Enter task description: ");
            echo();
            getstr(input);
            noecho();
            add_task(&tasks, input);
            task_count++;
        }
        else if (ch == 't')
        {
            mark_task_complete(tasks, selected_task);
        }
        else if (ch == 'd')
        {
            delete_task(&tasks, selected_task);
            if (selected_task >= task_count - 1)
            {
                selected_task = task_count - 2;
            }
            task_count--;
            if (task_count < 0)
            {
                task_count = 0;
            }
        }
        else if (ch == KEY_UP)
        {
            selected_task = (selected_task - 1 + task_count) % task_count;
        }
        else if (ch == KEY_DOWN)
        {
            selected_task = (selected_task + 1) % task_count;
        }
    }

    end_ui();

    // Free memory
    Task *current = tasks;
    while (current != NULL)
    {
        Task *temp = current;
        current = current->next;
        free(temp->description);
        free(temp);
    }

    return 0;
}