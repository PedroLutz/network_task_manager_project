#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../general/structs.h"
#include "../general/sendReq.h"
#include "../general/ui.h"
#include <ctype.h>
#include "taskUI.h"

static bool isInteger(const char *str)
{
    if (str == NULL || *str == '\0')
        return false;

    for (int i = 0; str[i]; i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

bool createTaskUI(TASK *task)
{
    char warning[50] = "";
    TASK t;

    while (1)
    {
        cleanScreen();
        mprintf("Press ESC to exit\n");
        mprintf("Task description: ");
        printf("\033[s");
        printf("\n");
        mprintf("Priority (1 to 4): \n");

        printf("\n\033[2B\033[5D");
        if (strlen(warning) > 0)
        {
            mprintf("%s\n", warning);
            
            strcpy(warning, "");
            strcpy(t.description, "");
        }

        if (!getInputLine(t.description, sizeof(t.description)))
        {
            return false;
        }

        if (strlen(t.description) == 0)
        {
            strcpy(warning, "Please write a valid description!");
            continue;
        }

        break;
    }

    strcpy(warning, "");

    while (1)
    {
        cleanScreen();
        mprintf("Press ESC to exit\n");
        char priority[50];

        mprintf("Task description: %s\n", t.description);
        mprintf("Priority (1 to 4): ");
        printf("\033[s");

        printf("\n\033[2B\033[5D");
        if (strlen(warning) > 0)
        {
            mprintf("%s\n", warning);
            strcpy(warning, "");
            strcpy(priority, "");
        }

        printf("\033[u");

        if (!getInputLine(priority, sizeof(priority)))
        {
            return false;
        }

        if (strlen(priority) == 0)
        {
            strcpy(warning, "Please select a valid priority!");
            continue;
        }

        if (!isInteger(priority))
        {
            strcpy(warning, "Only numbers are allowed for the interval!");
            continue;
        }

        if (atoi(priority) < 1 || atoi(priority) > 4)
        {
            strcpy(warning, "Please select a priority inside the interval!");
            continue;
        }

        strcpy(task->description, t.description);
        task->priority = atoi(priority);
        break;
    }

    return true;
}

int generateTaskMenuFromRB(NODE *root, int numNodes){
    int option = 0;
    printf("\033[H");
    cleanScreen();
    printf("\033[s");
    mprintf("Exit\n");
    for(int i = 1; i <= numNodes; i++){
        int counter = 0;
        NODE *node = treeGetNodeByIndex(root, &counter, i);
        mprintf("[%d] P%d - %s\n",node->task->id, node->task->priority, node->task->description);
    }
    printf("\033[2B");
    mprintf("Use arrow up or down to select the option");
    printf("\033[u");
    printf("\033[32m  ->\033[0m");

    disableEcho();

    while(1){  
        char c = getchar();
        printf("\033[5D");
        if(c == '\033'){
            char c2 = getchar();
            char c3 = getchar();
            if (c2 == '[') {
                if (c3 == 'A') {
                    if(option > 0){
                        option--;
                        printf("     \r\033[1A\033[32m  ->\033[0m");
                    }
                } else if (c3 == 'B') {
                   if(option < numNodes){
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