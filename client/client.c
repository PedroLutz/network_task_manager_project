#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
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
        "Log in to existing account",
        "Create new account",
        "Exit");

    char *msg = NULL;
    switch(option){   
        case 1: 
        if(login(&msg, &loggedUser)){
            if(msg != NULL){
                printMessage(msg);
                free(msg);
            }
        }      
        break;

        case 2:
        if(createAccount(&msg)){
            printMessage(msg);
            free(msg);
        };
        
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

        char *msg;

        switch(option){
            case 1:
            if(createTask(loggedUser.id, &tree_tasksSortedById, &msg)){
                printMessage(msg);
                free(msg);
            }
            break;

            case 2:
            viewAllTasks(&tree_tasksSortedById);
            break;

            case 3:
            searchTasks(&tree_tasksSortedById);
            break;

            case 4:
            if(updateAccount(&loggedUser, &msg)){
                printMessage(msg);
                free(msg);
            };
            break;

            case 5:
            if(deleteAccount(&loggedUser, &msg)){
                printMessage(msg);
                free(msg);
                treeFree(tree_tasksSortedById);
                loggedUser.id = -1;
            }
            break;

            case 6:
            treeFree(tree_tasksSortedById);
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

        tree_tasksSortedById = NULL;

        char *err;
        if(!taskGetUserTasks(loggedUser.id, &tree_tasksSortedById, &err)){
            printMessage(err);
            free(err);
            exit(0);
        }

        while(loggedUser.id != -1){
            loggedInMenu();
        }
    }

}
