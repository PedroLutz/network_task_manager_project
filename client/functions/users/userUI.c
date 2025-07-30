#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../general/ui.h"
#include "../general/sendReq.h"
#include "../general/structs.h"

void loginUI(char *userName, char *password){
    char warning[50] = "";

    while(1){
        cleanScreen();
        
        printf("\033[s"); //save current terminal position
        printf("\033[2B"); //move two lines down
        if(strlen(warning) > 0){
            mprintf("%s\n", warning);
            strcpy(warning, "");
            strcpy(userName, "");
        }

        printf("\033[u"); //return to saved terminal position

        mprintf("Username: ");
        getText(userName, 50);

        if(strlen(userName) == 0){
            strcpy(warning, "Please write a valid username!");
        } else {
            break;
        }
    }

    while (1) {
        cleanScreen();

        mprintf("Username: %s\n", userName);

        printf("\033[s");

        printf("\033[2B"); 
        if (strlen(warning) > 0) {
            mprintf("%s", warning);
        }

        printf("\033[u");

        mprintf("Password: ");
        getText(password, 50);

        if (strlen(password) == 0) {
            strcpy(warning, "Please write a valid password!");
            continue;
        }

        break;
    }
}

void createAccountUI(char *userName, char *password, char *r_password){
    char warning[100] = "";

    while(1){
        cleanScreen();

        printf("\033[s");
        printf("\033[2B"); 
        if(strlen(warning) > 0){
            mprintf("%s\n", warning);
            strcpy(warning, "");
            strcpy(userName, "");
        }

        printf("\033[u");

        mprintf("Username: ");
        getText(userName, 50);

        if(strlen(userName) == 0){
            strcpy(warning, "Please write a valid username!");
        } else {
            break;
        }
    }

    strcpy(warning, "");

    while(1){
        cleanScreen();

        mprintf("Username: %s\n", userName);

        printf("\033[s");
        printf("\033[2B"); 
        if(strlen(warning) > 0){
            mprintf("%s\n", warning);
            strcpy(warning, "");
            strcpy(password, "");
            strcpy(r_password, "");
        }
        printf("\033[u");

        mprintf("Password: ");
        getText(password, 50);

        if(strlen(password) == 0){
            strcpy(warning, "Please write a valid password!");
            continue;
        }

        mprintf("Repeat your password: ");
        getText(r_password, 50);

        if(strcmp(password, r_password) != 0){
            strcpy(warning, "Please make sure your password is the same in both fields!");
        } else{
            break;
        }
    }
}