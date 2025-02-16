#ifndef TO_DO_APP_H
#define TO_DO_APP_H
#include "doubly_linked_list.h"
#include "hash_map.h"

struct Time;
struct SubTask;
struct Notes;
struct Appointment;
struct Task;
struct Calendar;
typedef enum RecurrenceType
{
    NONE,
    HOURLY,
    DAILY,
    BI_WEEKLY,
    WEEKLY,
    MONTHLY,
    YEARLY
}RecurrenceType;

typedef enum MonthsInAYear
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
} MonthsInAYear;

typedef enum CompletionLevel
{
    not_complete,
    pending,
    is_complete
}CompletionLevel;

typedef struct Time
{
    char minutes;
    char hour;
}Time;

typedef struct SubTask
{
    char *description;
    unsigned int priority;
}SubTask;

typedef struct Notes
{
    char *description;
}Notes;

typedef struct Appointment Appointment;

typedef struct Appointment
{
    int id;
    char date;
    Time time;
    MonthsInAYear months;
    Notes notes;
    bool is_complete;
    bool have_reminder;
    RecurrenceType recurrence_type;
    struct Task *task;  
}Appointment;

typedef struct Calendar
{
    char date;
    char year;
    Time time;
    MonthsInAYear months;
    struct Appointment *appointment;
    struct Task *task;
}Calendar;

typedef struct Task 
{
    int id;
    char *description;
    bool is_complete;
    bool have_subtask;
    bool have_reminder;
    unsigned int priority;
    RecurrenceType recurrence_type;
    Time time;
    char date;
    Notes notes;
    SubTask sub_task;
    Appointment *appointment;
} Task;

Task *create_task(DoublyLinkedList *list, HashMap *map, int id, const char *description, unsigned int priority);

SubTask *create_subtask(DoublyLinkedList *list, HashMap *map, int task_id, const char *description, unsigned int priority);

Node *add_task_to_calendar(Calendar *calendar, DoublyLinkedList *list, HashMap *map, int task_id);

Task *update_task(DoublyLinkedList *list, HashMap *map, int task_id, const char *new_description, unsigned int new_priority);

SubTask *update_subtask(DoublyLinkedList *list, HashMap *map, int task_id, const char *description, unsigned int priority);

bool delete_task(DoublyLinkedList *list, HashMap *map, int task_id);

bool delete_sub_task(DoublyLinkedList *list, HashMap *map, int task_id);

Appointment *create_appointment(DoublyLinkedList *list, HashMap *map, int id, char date, Time time, MonthsInAYear month, Notes notes, bool have_reminder, RecurrenceType recurrence);

Node *add_appointment_to_calendar(Calendar *calendar, DoublyLinkedList *list, HashMap *map, int appointment_id);

Appointment *update_appointment(DoublyLinkedList *list, HashMap *map, int appointment_id, char new_date, Time new_time, MonthsInAYear new_month, Notes new_notes, bool new_have_reminder, RecurrenceType new_recurrence);

bool delete_appointment(DoublyLinkedList *list, HashMap *map, int appointment_id);

void link_task_to_appointment(Task *task, Appointment *appointment);

Calendar *create_calendar(char date, char year, Time time, MonthsInAYear months, struct Appointment *appointment, struct Task *task);

Calendar *update_calendar(char date, char year, Time time, MonthsInAYear months, struct Appointment *appointment, struct Task *task);

#endif