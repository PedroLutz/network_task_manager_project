#ifndef UI_H
#define UI_H

#define MARGIN 5

void disableEcho(void);
void enableEcho(void);
int generateMenu(int numOptions, ...);
void mprintf(const char *format, ...);
void cleanScreen(void);
void printLeftMargin(void);
void cleanBuffer(void);
void getText(char *str, int size);
void printMessage(char *msg);

#endif