#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include "./functions/general/structs.h"
#include "./functions/users/userData.h"
#include "./functions/general/ui.h"
#include "./functions/general/sendReq.h"
#include "./functions/tasks/taskRbTree.h"
#include "./functions/tasks/taskData.h"

USER loggedUser;
NODE *tree_tasksSortedById = NULL;

void initUser(){
    loggedUser.id = -1;
    strcpy(loggedUser.userName, "");
    strcpy(loggedUser.password, "");
}

void loggedOutMenu(void){
    int option = generateMenu(3,
        "Login to existing account",
        "Create new account",
        "Exit");

    char *msg = NULL;
    switch(option){   
        case 1: 
        if(!login(&msg, &loggedUser)){
            printMessage(msg);
            free(msg);
        }      
        break;

        case 2:
        createAccount(&msg);
        printMessage(msg);
        free(msg);
        break;

        case 3:
        exit(0);
        break;

        default:
        break;
    }
}

void loggedInMenu(void){
    int option = generateMenu(6 ,
        "Register new task",
        "View all tasks",
        "Search for tasks",
        "Update user data",
        "Delete user data",
        "Exit");

        switch(option){
            case 5:
            break;

            case 6:
            exit(0);
            break;

            default:
            break;
        }
}

int main(){
    initUser();

    while(1){
        while(loggedUser.id == -1){
            loggedOutMenu();
        }

        char *err;
        if(!taskGetUserTasks(loggedUser.id, &tree_tasksSortedById, &err)){
            printMessage(err);
            exit(0);
        }

        while(loggedUser.id != -1){
            loggedInMenu();
        }
    }
}