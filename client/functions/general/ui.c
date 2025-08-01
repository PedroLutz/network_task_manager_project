#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "ui.h"
#include <stdarg.h>
#include <termios.h>
#include <unistd.h>
#include "../tasks/taskRbTree.h"

static struct termios oldt;

void disableEcho(void) {
    printf("\033[?25l");
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt); 
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void enableEcho() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\033[?25h");
}

char getch() {
    struct termios oldt, newt;
    char ch;

    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); 
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    read(STDIN_FILENO, &ch, 1);

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); 
    return ch;
}

bool getInputLine(char *buffer, size_t size) {
    size_t index = 0;
    printf("\033[s");
    printf("\033[u \b");
    fflush(stdout);
    
    while (1) {
        char ch = getch();

        if (ch == ESC) {
            return false;
        }

        if (ch == ENTER) {
            buffer[index] = '\0';
            printf("\n");
            return true; 
        }

        if (ch == BACKSPACE) {
            if (index > 0) {
                index--;
                buffer[index] = '\0';
                printf("\b \b");
                fflush(stdout);
            }
            continue;
        }

        if (index < size - 1 && ch >= 32 && ch <= 126) {
            buffer[index++] = ch;
            buffer[index] = '\0';
            printf("%c", ch);
            fflush(stdout);
        }
    }
}

int generateMenu(int numOptions, ...){
    va_list args;
    va_start(args, numOptions);

    int option = 1;
    printf("\033[H");
    cleanScreen();
    printf("\033[s");
    for(int i = 0; i < numOptions; i++){
        const char *text = va_arg(args, const char *); 
        mprintf("%s\n", text);
    }
    printf("\033[2B");
    mprintf("Use arrow up or down to select the option");
    printf("\033[u");
    printf("\033[32m  ->\033[0m");

    va_end(args);
    disableEcho();

    while(1){  
        char c = getchar();
        printf("\033[5D");
        if(c == '\033'){
            char c2 = getchar();
            char c3 = getchar();
            if (c2 == '[') {
                if (c3 == 'A') {
                    if(option > 1){
                        option--;
                        printf("     \r\033[1A\033[32m  ->\033[0m");
                    }
                } else if (c3 == 'B') {
                   if(option < numOptions){
                        option++;
                        printf("     \r\033[1B\033[32m  ->\033[0m");
                    }
                }
            }   
        } else if (c == '\n'){
            enableEcho();
            return option;
        }
    }
}

bool confirmationMenu(char *label){
    int option = 1;
    printf("\033[H");
    cleanScreen();
    mprintf("%s\n\n", label);
    printf("\033[s");
    mprintf("Yes\n");
    mprintf("No\n");
    printf("\033[2B");
    mprintf("Use arrow up or down to select the option");
    printf("\033[u");
    printf("\033[32m  ->\033[0m");

    disableEcho();

    while (1)
    {
        char c = getchar();
        printf("\033[5D");
        if (c == '\033')
        {
            char c2 = getchar();
            char c3 = getchar();
            if (c2 == '[')
            {
                if (c3 == 'A')
                {
                    if (option > 1)
                    {
                        option--;
                        printf("     \r\033[1A\033[32m  ->\033[0m");
                    }
                }
                else if (c3 == 'B')
                {
                    if (option < 2)
                    {
                        option++;
                        printf("     \r\033[1B\033[32m  ->\033[0m");
                    }
                }
            }
        }
        else if (c == '\n')
        {
            enableEcho();
            break;
        }
    }

    return option == 1;
}

void cleanScreen(void){
    system("clear");
    printf("\n\n");
}

void printLeftMargin(void){
    printf("\033[%dC", MARGIN);
}

void mprintf(const char *format, ...) {
    printLeftMargin();  

    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
}

void cleanBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void getText(char *str, int size)
{
    fgets(str, size, stdin);
    str[strcspn(str, "\n")] = '\0';
}

void printMessage(char *msg){
    cleanScreen();
    mprintf("%s\n", msg);
    mprintf("Press anything to continue.\n");
    disableEcho(); getchar(); enableEcho();
}

