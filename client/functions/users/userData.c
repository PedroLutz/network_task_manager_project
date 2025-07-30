#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../general/ui.h"
#include "../general/sendReq.h"
#include "../general/structs.h"
#include "userUI.h"

bool login(char **err, USER *user){
    char userName[50];
    char password[50];

    loginUI(userName, password);

    char reqData[256]; 
    snprintf(reqData, sizeof(reqData), "userName:%s;password:%s", userName, password);
    char *req;

    if(!buildReq(TABLE_USERS, MODE_GET, reqData, &req)){
        *err = strdup("Error while generating request!");
        return false;
    }

    char *res;
    if(!sendReq(req, &res)){
        *err = strdup("Error while sending the request!");
        free(req);
        return false;
    }

    if(strcmp(res, "userName_notFound") == 0){
        *err = strdup("This username is not registered!");
        free(req); free(res); 
        return false;
    }

    if(strcmp(res, "password_wrong") == 0){
        *err = strdup("Wrong password!");
        free(req); free(res); 
        return false;
    }

    char *token;
    token = strtok(res, "|");
    if(strcmp(token, "password_ok") == 0){
        token = strtok(NULL, "\0");
        int id;
        sscanf(token, "id:%d", &id);
        user->id = id;
        strcpy(user->userName, userName);
        strcpy(user->password, password);
    } else {
        *err = strdup("Error while receiving server response!");
        free(req); free(res); 
        return false;
    }

    free(req);
    free(res);
    return true;
}

void createAccount(char **msg){
    char userName[50];
    char password[50];
    char r_password[50];

    createAccountUI(userName, password, r_password);

    char reqData[256]; 
    snprintf(reqData, sizeof(reqData), "userName:%s;password:%s", userName, password);
    char *req;

    if(!buildReq(TABLE_USERS, MODE_CREATE, reqData, &req)){
        *msg = strdup("Error while generating request!");
        return;
    }

    char *res;
    if(!sendReq(req, &res)){
        *msg = strdup("Error while sending the request!");
        free(req);
        return;
    }

    if(strcmp(res, "userName_exists") == 0){
        *msg = strdup("This username is already registered!");
        free(req); free(res); 
        return;
    }

    if(strcmp(res, "create_fail") == 0){
        *msg = strdup("Error while creating the account!");
        free(req); free(res); 
        return;
    }

    if(strcmp(res, "create_success") == 0){
        *msg = strdup("Account created successfully!");
        free(req); free(res); 
        return;
    }

    *msg = strdup("Error while receiving server response!");
    free(req); free(res); 
    return;
}