#ifndef TASKUI_H
#define TASKUI_H

#include <stdbool.h>

bool createTaskUI(TASK *task);
int generateTaskMenuFromRB(NODE *root, int numNodes);

#endif