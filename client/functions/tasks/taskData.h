#ifndef TASKDATA_H
#define TASKDATA_H

#include "taskRbTree.h"
#include <stdbool.h>

bool taskGetUserTasks(int userId, NODE **root, char **err);

#endif