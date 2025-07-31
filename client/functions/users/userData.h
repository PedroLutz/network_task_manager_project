#ifndef USERACCFUNCTIONS_H
#define USERACCFUNCTIONS_H

#include "../general/structs.h"
#include <stdbool.h>

bool login(char **err, USER *user);
bool createAccount(char **msg);

#endif