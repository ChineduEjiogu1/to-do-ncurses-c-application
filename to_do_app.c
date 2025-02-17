#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "to_do_app.h"
#include "hash_map.h"
#include "hash_map_api.c"
#include "doubly_linked_list.h"
#include "doubly_linked_list_api.c"
#include "bst.h"
#include "bst.c"

Task *create_task(BST *task_tree, DoublyLinkedList *list, HashMap *map, int id, const char *description, unsigned int priority)
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
        printf("Memory allocation failed for task description\n");
        free(new_task);
        free(new_data);
        return NULL;
    }

    new_task->id = id;
    new_task->priority = priority;
    new_data->value = new_task;

    // Insert into DLL (for ordered list handling)
    Node *new_node = insert_by_position(list, id, new_data);
    if (!new_node)
    {
        printf("Memory allocation failed for creating node\n");
        free(new_task->description);
        free(new_task);
        free(new_data);
        return NULL;
    }

    // Insert into Hash Map for fast ID lookup
    if (!insert_into_hash_map(map, id, new_node))
    {
        printf("Error: Failed to insert task into hash map\n");
        free(new_task->description);
        free(new_task);
        free(new_data);
        free(new_node);
        return NULL;
    }

    // Insert into BST based on priority
    int *priority_key = (int *)malloc(sizeof(int));
    if (!priority_key)
    {
        printf("Memory allocation failed for priority key\n");
        free(new_task->description);
        free(new_task);
        free(new_data);
        return NULL;
    }

    *priority_key = priority;
    add_to_bst(task_tree, priority_key, new_task);

    return new_task;
}

SubTask *create_subtask(DoublyLinkedList *list, HashMap *map, int task_id, const char *description, unsigned int priority)
{
    SubTask *new_sub_task = (SubTask *)malloc(sizeof(SubTask));

    if (!new_sub_task)
    {
        printf("Memory allocation failed for creating subtask\n");
        return NULL;
    }

    Data *new_data = (Data *)malloc(sizeof(Data));

    if (!new_data)
    {
        printf("Memory allocation failed for creating data\n");
        free(new_sub_task);
        return NULL;
    }

    new_sub_task->description = strdup(description);

    if (!new_sub_task->description)
    {
        printf("Memory allocation failed for subtask description\n");
        free(new_sub_task);
        free(new_data);
        return NULL;
    }

    new_sub_task->priority = priority;
    new_data->value = new_sub_task;

    // Find the parent task in the hash map
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

    // If this is the first subtask, initialize the list
    if (!task->subtasks)
    {
        task->subtasks = create_doubly_linked_list();
    }

    // Insert subtask into the list (maintains priority order)
    insert_by_position(task->subtasks, priority, new_data);

    task->have_subtask = true; // Mark task as having subtasks

    return new_sub_task;
}

void add_task_to_calendar(Calendar *calendar, BST *task_bst, HashMap *map, int task_id)
{
    Node *task_node = find_hash_entry(map, task_id);

    if (!task_node)
    {
        printf("Error: Task with ID %d not found\n", task_id);
        return;
    }

    Task *task = (Task *)task_node->data->value;

    // Insert task into the BST (ordered by priority)
    add_to_bst(task_bst, &task->priority, task);

    // Link task to calendar
    calendar->task = task;
}

Task *update_task(BST *task_bst, HashMap *map, int task_id, const char *new_description, unsigned int new_priority,
                  void (*free_data)(void *), void (*free_key)(void *), void *(*copy_key)(void *), void *(*copy_data)(void *))
{
    // Find the task node in HashMap
    Node *task_node = find_hash_entry(map, task_id);
    
    if (!task_node)
    {
        printf("Error: Task with ID %d not found\n", task_id);
        return NULL;
    }

    Task *task = (Task *)task_node->data->value;

    // Remove from BST before updating priority
    task_bst->root = delete_bst_node(task_bst->root, &task->priority, free_data, free_key, copy_key, copy_data, task_bst->cmp);

    // Free old description and update task
    free(task->description);
    task->description = strdup(new_description);
    task->priority = new_priority;

    // Reinsert task into BST with updated priority
    add_to_bst(task_bst, &task->priority, task);

    return task;
}

SubTask *update_subtask(DoublyLinkedList *list, HashMap *map, int task_id, const char *description, unsigned int priority)
{

}