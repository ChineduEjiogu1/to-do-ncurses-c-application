#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "to_do_app.h"
#include "hash_map.h"
#include "hash_map_api.c"
#include "doubly_linked_list.h"
#include "doubly_linked_list_api.c"

Task *create_task(DoublyLinkedList *list, HashMap *map, int id, const char *description, unsigned int priority)
{
    Task *new_task = (Task *)calloc(1, sizeof(Task));

    if (!new_task)
    {
        printf("Memory allocation failed for creating task\n");
        return NULL;
    }

    Data *new_data = (Data *)malloc(sizeof(Data));
    if (!new_data)
    {
        printf("Memory allocation failed for creating data\n");
        free(new_task);
        return NULL;
    }

    new_task->description = strdup(description);

    if (!new_task->description)
    {
        printf("Memory allocation failed for creating task description\n");
        free(new_task);
        free(new_data);
        return NULL;
    }

    new_task->id = id;
    new_task->priority = priority;
    new_data->value = new_task;

    Node *new_node = insert_by_position(list, id, new_data);

    if (!new_node)
    {
        printf("Memory allocation failed for creating node\n");
        free(new_task->description);
        free(new_task);
        free(new_data);
        return NULL;
    }

    if (!insert_into_hash_map(map, id, new_node))
    {
        printf("Error: Failed to insert task into hash map\n");
        free(new_task->description);
        free(new_task);
        free(new_data);
        free(new_node);
        return NULL;
    }

    return new_task;
}

SubTask *create_subtask(DoublyLinkedList *list, HashMap *map, int task_id, const char *description, unsigned int priority)
{
    SubTask *new_sub_task = (SubTask *)malloc(sizeof(SubTask));

    Data *new_data = (Data *)malloc(sizeof(Data));

    if (!new_sub_task)
    {
        printf("Memory allocation failed for creating sub task\n");
        return NULL;
    }

    if (!new_data)
    {
        printf("Memory allocation failed for creating data\n");
        free(new_sub_task);
        return NULL;
    }

    new_sub_task->description = strdup(description);
    if (!new_sub_task->description)
    {
        printf("Memory allocation failed for creating task description\n");
        free(new_sub_task);
        free(new_data);
        return NULL;
    }

    new_sub_task->priority = priority;
    new_data->value = new_sub_task;

    // Find the parent task
    Node *task_node = find_hash_entry(map, task_id);
    if (!task_node)
    {
        printf("Error: Task with ID %d not found\n", task_id);
        free(new_sub_task->description);
        free(new_sub_task);
        free(new_data);
        return NULL;
    }

    Task *task = (Task *)task_node->data->value;

    // Attach subtask to the parent task
    task->sub_task = *new_sub_task;
    task->have_subtask = true; // Mark task as having a subtask

    insert_by_position(list, task_id, new_data);

    return new_sub_task;
}

Node *add_task_to_calendar(Calendar *calendar, DoublyLinkedList *list, HashMap *map, int task_id)
{
    Node *new_node = NULL;

    Data *new_data = (Data *)malloc(sizeof(Data));

    if (!new_data)
    {
        printf("Memory allocation failed for creating data\n");
        return NULL;
    }

    Node *task_node = find_hash_entry(map, task_id);

    if (!task_node)
    {
        printf("Error: Task with ID %d not found\n", task_id);
        free(new_data);
        return NULL;
    }

    Task *task = (Task *)task_node->data->value;

    new_data->value = task;

    new_node = insert_by_position(list, task_id, new_data);

    return new_node;
}

Task *update_task(DoublyLinkedList *list, HashMap *map, int task_id, const char *new_description, unsigned int new_priority)
{
    Node *node = find_hash_entry(map, task_id);

    if (!node)
    {
        printf("Error: Task with ID %d not found\n", task_id);
        return NULL;
    }

    Task *task = (Task *)node->data->value;

    // Free old description before assigning new one
    if (task->description)
    {
        free(task->description);
    }
    task->description = strdup(new_description);
    task->priority = new_priority;

    Data *new_data = (Data *)malloc(sizeof(Data));
    if (!new_data)
    {
        printf("Memory allocation failed for creating data\n");
        return NULL;
    }
    new_data->value = task; // Wrap task inside Data

    // Move task down if necessary
    if (node->next && ((Task *)node->next->data->value)->priority < task->priority)
    {
        delete_by_value(list, task_id);
        node = insert_by_position(list, task_id, new_data);
    }
    // Move task up if necessary
    else if (node->previous && ((Task *)node->previous->data->value)->priority > task->priority)
    {
        delete_by_value(list, task_id);
        node = insert_by_position(list, task_id, new_data);
    }

    return (Task *)node->data->value;
}

SubTask *update_subtask(DoublyLinkedList *list, HashMap *map, int task_id, const char *description, unsigned int priority)
{
    
}