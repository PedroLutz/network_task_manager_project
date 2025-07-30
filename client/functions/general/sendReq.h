#ifndef SENDREQ_H
#define SENDREQ_H

#include <stdbool.h>

#define TABLE_USERS "users"
#define TABLE_TASKS "tasks"
#define MODE_CREATE "create"
#define MODE_GET "get"
#define MODE_DELETE "delete"
#define MODE_UPDATE "update"

bool sendReq(char *req, char **res);
char *buildReq(char *table, char *mode, char *req, char **returnReq);

#endif