#ifndef USERACCFUNCTIONS_H
#define USERACCFUNCTIONS_H

#include "../general/structs.h"
#include <stdbool.h>

bool login(char **err, USER *user);
bool createAccount(char **msg);
bool updateAccount(USER *user, char **msg);
bool deleteAccount(USER *user, char **msg);

#endif