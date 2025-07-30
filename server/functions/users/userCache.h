#ifndef USERCACHE_H
#define USERCACHE_H

#include "../general/structs.h"

void userCache_init(void);
bool userCache_insert(USER user);
void userCache_update(USER newData);
bool userCache_search(const char *userName);
void userCache_checkLogin(USER user, char **res);
void userCache_free(void);

#endif