#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <stdbool.h>
#include "task_manager.h"  // Assuming this contains TaskManager, Task, etc.

// --- Core Persistence Functions --- //

// Save all tasks, appointments, and relevant data to a binary file
bool save_tasks_to_file(TaskManager *manager, const char *filename);

// Load tasks and appointments from a binary file into the TaskManager
bool load_tasks_from_file(TaskManager *manager, const char *filename);

// Save the last used task/appointment ID to ensure no duplicates
bool save_last_used_id(const char *filename, int last_id);

// Load the last used ID from file
int load_last_used_id(const char *filename);

// --- Utility Functions for Serialization --- //

// Helper to serialize a task into the file
bool serialize_task(FILE *file, Task *task);

// Helper to deserialize a task from the file
Task *deserialize_task(FILE *file);

#endif // PERSISTENCE_H
