#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../general/structs.h"
#include "../general/modeParser.h"
#include "taskDb.h"

void handleTaskReq(char *buffer, char **response){
    char *modeStr = strtok(buffer, "|");
    char *data = strtok(NULL, "");

    char operation[20];
    sscanf(modeStr, "mode:%19s", operation);

    int id = -1;
    int userId = -1;
    char description[100] = "";
    STATUS status = 1;
    int priority = -1;
    
    if(data != NULL){
        char *token = strtok(data, ";");
        while(token != NULL){
            if (sscanf(token, "id:%d", &id)) {}
            else if (sscanf(token, "userId:%d", &userId)) {}
            else if (sscanf(token, "description:%99[^;]", description)) {}
            else if (sscanf(token, "status:%d", (int *)&status)) {}
            else if (sscanf(token, "priority:%d", &priority)) {}
            token = strtok(NULL, ";");
        }
    }

    MODE parsedMode = parseMode(operation);

    TASK task;
    task.id = id;
    task.userId = userId;
    strncpy(task.description, description, sizeof(task.description));
    task.description[sizeof(task.description) - 1] = '\0';
    task.status = status;
    task.priority = priority;

    switch(parsedMode){
        case MODE_CREATE: {
            int lastId = fileTaskFindLastId();
            if(lastId < 0) lastId = 0;
            task.id = lastId + 1;
            if(fileTaskCreate(&task)){
                *response = strdup("create_success");
                fileTaskSetLastId(task.id);
            } else {
                *response = strdup("create_fail");
            }
            break;
        }
        case MODE_UPDATE: {
            if(fileTaskUpdate(&task, id)){
                *response = strdup("update_success");
            } else {
                *response = strdup("update_fail");
            }
            break;
        }
        case MODE_DELETE:
            if(fileTaskDeleteById(id)){
                *response = strdup("delete_success");
            } else {
                *response = strdup("delete_fail");
            }
            break;
        case MODE_GET: {
            char *data = fileTaskGetByUser(userId);
            if(data){
                *response = strdup(data);
                free(data);
            } else {
                *response = strdup("get_fail");
            }
            break;
        }
        default:{
            puts("Invalid request");
            *response = strdup("invalid_request");
        }
    }
}
