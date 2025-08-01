#ifndef TASKUI_H
#define TASKUI_H

#include <stdbool.h>
#include "../general/structs.h"
typedef bool (*TaskCondition)(TASK *task, void *context);

bool createTaskUI(TASK *task);
int generateTaskMenuFromRB(NODE *root, int numNodes);
bool searchByIdUI(int *targetId);
bool searchByKeywordUI(char targetKeyword[100]);

#endif