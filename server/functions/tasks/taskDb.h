#ifndef TASKDB_H
#define TASKDB_H

int fileTaskFindLastId(void);
void fileTaskSetLastId(int id);
void fileTaskFindLoop(TASK *task, int id, unsigned short int *position, bool *found, FILE *fp);
bool fileTaskFindPositionById(int id, unsigned short int *returnPosition);
char *fileTaskFindById(int id, char *returnBuffer);
char* fileTaskGetByUser(int userId); //the caller needs to free memory for the buffer after using it
bool fileTaskCreate(TASK *task);
bool fileTaskUpdate(TASK *task, int id);
bool fileTaskDeleteById(int id);
bool fileTaskDeleteByUser(int userId);

#define TASK_ID_POS sizeof(int)

#endif