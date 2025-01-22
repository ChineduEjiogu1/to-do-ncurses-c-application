#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

// Task structure
typedef struct Task {
    char *description; // Task description
    char *note;        // Note associated with the task
    bool is_complete;  // Completion status
} Task;

// Appointment structure
typedef struct Appointment {
    char *time;        // Appointment time
    char *description; // Appointment description
    int day;           // Day of the month for the appointment
} Appointment;

// Sample tasks
Task tasks[] = {
    {"Task 1 - High Priority", "This is a note for Task 1.", false},
    {"Task 2 - Medium Priority", "This is a note for Task 2.", false},
    {"Task 3 - Low Priority", "This is a note for Task 3.", true},
};
int task_count = sizeof(tasks) / sizeof(tasks[0]);

// Sample appointments
Appointment appointments[] = {
    {"10:00 AM", "Meeting with Client", 10},
    {"12:00 PM", "Lunch with Sarah", 12},
    {"3:00 PM", "Conference Call", 15},
};
int appointment_count = sizeof(appointments) / sizeof(appointments[0]);

// Function to draw a border with a title
void draw_borders_with_title(WINDOW *win, const char *title) {
    box(win, 0, 0); // Draw the border
    int width = getmaxx(win);
    mvwprintw(win, 0, (width - strlen(title)) / 2, " %s ", title); // Center the title
    wrefresh(win);
}

// Function to display tasks
void display_tasks(WINDOW *tasks_win, int highlight) {
    wclear(tasks_win);
    draw_borders_with_title(tasks_win, "Tasks");
    for (int i = 0; i < task_count; ++i) {
        if (i == highlight) {
            wattron(tasks_win, A_REVERSE);
        }
        mvwprintw(tasks_win, i + 1, 2, "[%c] %s", tasks[i].is_complete ? 'x' : ' ', tasks[i].description);
        if (i == highlight) {
            wattroff(tasks_win, A_REVERSE);
        }
    }
    wrefresh(tasks_win);
}

// Function to display notes for the highlighted task
void display_note(WINDOW *notes_win, const char *note) {
    wclear(notes_win);
    draw_borders_with_title(notes_win, "Notes");
    mvwprintw(notes_win, 1, 2, "%s", note ? note : "No notes available.");
    wrefresh(notes_win);
}

// Function to display the calendar and integrate appointments
void display_calendar(WINDOW *calendar_win, int selected_day) {
    wclear(calendar_win);
    draw_borders_with_title(calendar_win, "Calendar");
    mvwprintw(calendar_win, 1, 2, "January 2025");
    mvwprintw(calendar_win, 2, 2, "Mon  Tue  Wed  Thu  Fri  Sat  Sun");

    // Days of the month (January 2025 starts on a Thursday)
    mvwprintw(calendar_win, 3, 2, "  1    2    3    4    5    6    7");
    mvwprintw(calendar_win, 4, 2, "  8    9   10   11   12   13   14");
    mvwprintw(calendar_win, 5, 2, " 15   16   17   18   19   20   21");
    mvwprintw(calendar_win, 6, 2, " 22   23   24   25   26   27   28");
    mvwprintw(calendar_win, 7, 2, " 29   30   31");

    // Highlight the selected day
    int row = 3 + (selected_day - 1) / 7; // Calculate row based on the day of the month
    int col = 2 + ((selected_day - 1) % 7) * 6; // Calculate column for the specific day
    wattron(calendar_win, A_REVERSE);
    mvwprintw(calendar_win, row, col, "%d", selected_day);
    wattroff(calendar_win, A_REVERSE);

    // Display appointments for the selected day (behind calendar)
    for (int i = 0; i < appointment_count; ++i) {
        if (appointments[i].day == selected_day) {
            mvwprintw(calendar_win, row + 1, col, "%s - %s", appointments[i].time, appointments[i].description);
        }
    }

    wrefresh(calendar_win);
}

int main() {
    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);

    // Screen dimensions
    int height, width;
    getmaxyx(stdscr, height, width);

    // Check terminal size
    if (height < 20 || width < 60) {
        endwin();
        printf("Terminal size too small! Please resize and run again.\n");
        return 1;
    }

    // Calculate window sizes
    int tasks_width = width / 3;
    int calendar_width = width - tasks_width;
    int notes_height = 5;

    // Create windows
    WINDOW *tasks_win = newwin(height - notes_height, tasks_width, 0, 0);
    WINDOW *calendar_win = newwin(height - notes_height, calendar_width, 0, tasks_width);
    WINDOW *notes_win = newwin(notes_height, width, height - notes_height, 0);

    // Initial display
    int selected_day = 1; // Start from the 1st day of the month
    display_tasks(tasks_win, 0); // Display tasks with first task highlighted
    display_calendar(calendar_win, selected_day); // Display the calendar with the selected day
    display_note(notes_win, tasks[0].note); // Display notes for the first task

    // Main loop to handle user input
    int ch;
    while ((ch = getch()) != 'q') {
        switch (ch) {
            case KEY_UP:
                if (selected_day > 7) selected_day -= 7; // Navigate up (move one week back)
                break;
            case KEY_DOWN:
                if (selected_day <= 24) selected_day += 7; // Navigate down (move one week forward)
                break;
            case KEY_LEFT:
                if (selected_day > 1) selected_day--; // Navigate left (move one day back)
                break;
            case KEY_RIGHT:
                if (selected_day < 31) selected_day++; // Navigate right (move one day forward)
                break;
        }
        display_tasks(tasks_win, 0); // Display tasks (unchanged)
        display_calendar(calendar_win, selected_day); // Display the updated calendar with the selected day
        display_note(notes_win, tasks[0].note); // Display notes (unchanged)
    }

    // Clean up
    delwin(tasks_win);
    delwin(calendar_win);
    delwin(notes_win);
    endwin();

    return 0;
}
