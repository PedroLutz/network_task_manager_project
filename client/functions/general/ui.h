#ifndef UI_H
#define UI_H

#include <stdbool.h>
#include "../tasks/taskRbTree.h"

#define MARGIN 5

#define ESC 27
#define ENTER '\n'
#define BACKSPACE 127

void disableEcho(void);
void enableEcho(void);
int generateMenu(int numOptions, ...);
void mprintf(const char *format, ...);
void cleanScreen(void);
void printLeftMargin(void);
void cleanBuffer(void);
void getText(char *str, int size);
void printMessage(char *msg);
bool getInputLine(char *buffer, size_t size);

#endif