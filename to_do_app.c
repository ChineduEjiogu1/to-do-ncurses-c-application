#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "to_do_app.h"
#include "hash_map.h"
#include "doubly_linked_list.h"
#include "doubly_linked_list_api.c"
#include "hash_map.h"

// Task* create_task(int id, const char* description, unsigned int priority) {
//     Task* task = calloc(1, sizeof(Task));
//     if (!task) {
//         printf("Memory allocation failed for task\n");
//         return NULL;
//     }
    
//     // Only set non-zero fields
//     task->description = strdup(description);
//     task->priority = priority;
    
//     return task;
// }