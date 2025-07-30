#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../general/structs.h"
#include "../general/sendReq.h"
#include "taskRbTree.h"
#include "taskData.h"

bool taskGetUserTasks(int userId, NODE **root, char **err){
    char reqData[20]; 
    snprintf(reqData, sizeof(reqData), "userId:%d", userId);
    char *req;

    if(!buildReq(TABLE_TASKS, MODE_GET, reqData, &req)){
        *err = strdup("Error while generating request!");
        return false;
    }

    char *res;
    if(!sendReq(req, &res)){
        *err = strdup("Error while sending the request!");
        free(req);
        return false;
    }

    if(strcmp(res, "get_fail") == 0){
        *err = strdup("Error while receiving the data!");
        free(req); free(res); 
        return false;
    }

    if(res[0] == '\0'){
        free(req); free(res);
        return true;
    }

    char *saveptr1;
    char *line = strtok_r(res, "\n", &saveptr1);
    while(line != NULL){
        if(strlen(line) == 0) { 
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }

        TASK *t = malloc(sizeof(TASK));
        char lineCopy[256];
        strncpy(lineCopy, line, sizeof(lineCopy));
        lineCopy[sizeof(lineCopy) - 1] = '\0';

        char *saveptr2;
        char *part = strtok_r(lineCopy, ";", &saveptr2);
        if(part == NULL) {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        t->id = atoi(part);

        part = strtok_r(NULL, ";", &saveptr2);
        if(part == NULL) {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        t->userId = atoi(part);

        part = strtok_r(NULL, ";", &saveptr2);
        if(part == NULL) {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        strcpy(t->description, part);

        part = strtok_r(NULL, ";", &saveptr2);
        if(part == NULL) {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        t->status = atoi(part);

        part = strtok_r(NULL, ";", &saveptr2);
        if(part == NULL) {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        t->priority = atoi(part);

        treeInsert(root, t, ID);

        line = strtok_r(NULL, "\n", &saveptr1);
    }

    free(req); free(res);
    return true;
}