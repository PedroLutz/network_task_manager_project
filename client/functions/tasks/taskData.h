#ifndef TASKDATA_H
#define TASKDATA_H

#include "taskRbTree.h"
#include <stdbool.h>

bool taskGetUserTasks(int userId, NODE **root, char **err);
bool createTask(int userID, NODE **root, char **msg);
void viewAllTasks(NODE **root);
void searchTasks(NODE **root);

#endif