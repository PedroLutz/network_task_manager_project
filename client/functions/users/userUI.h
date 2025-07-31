#ifndef USERUI_H
#define USERUI_H

#include <stdbool.h>

bool loginUI(char *userName, char *password);
bool createAccountUI(char *userName, char *password, char *r_password);

#endif