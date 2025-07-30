#ifndef USERDB_H
#define USERDB_H

#include "../general/structs.h"

void fileUserFindLoop(USER *user, int id, unsigned short int *position, bool *found, FILE *fp);
int fileUserFindLastId(void);
void fileUserSetLastId(int id);
bool fileUserFindPositionById(int id, unsigned short int *returnPosition);
bool fileUserUpdate(USER *user, int id);
bool fileUserCreate(USER *user);
bool fileUserDeleteById(int id);
char *fileUserGetAll(void);

#define USER_ID_POS 0

#endif