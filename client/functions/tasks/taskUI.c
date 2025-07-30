#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../general/structs.h"
#include "../general/sendReq.h"

static bool isInteger(const char *str) {
    if (str == NULL || *str == '\0') return false;

    for (int i = 0; str[i]; i++) {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

void createTaskUI(TASK *task){
    char warning[50] = "";

    while (1) {
        cleanScreen();
        mprintf("Task description: ");

        printf("\033[s");

        mprintf("\nPriority (1 to 4): ");
        

        printf("\033[2B"); 
        if (strlen(warning) > 0) {
            mprintf("%s", warning);
        }

        printf("\033[u");
        
        getText(task->description, sizeof(task->description));

        if (strlen(task->description) == 0) {
            strcpy(warning, "Please write a valid description!");
            continue;
        }

        break;
    }

    while (1) {
        cleanScreen();
        char priority[50];
        mprintf("Task description: %s\n", task->description);
        mprintf("Priority (1 to 4): ");

        printf("\033[s");
        printf("\033[2B"); 
        if (strlen(warning) > 0) {
            mprintf("%s", warning);
        }

        printf("\033[u");

        
        getText(priority, sizeof(priority));

        if (strlen(priority) == 0) {
            strcpy(warning, "Please select a valid priority!");
            continue;
        }

        if(atoi(priority) < 1 || atoi(priority) > 4){
            strcpy(warning, "Please select a priority inside the interval!");
            continue;
        }

        if (!isInteger(priority)) {
            strcpy(warning, "Only numbers are allowed for the interval!");
            continue;
        }

        task->priority = atoi(priority);

        break;
    }
}