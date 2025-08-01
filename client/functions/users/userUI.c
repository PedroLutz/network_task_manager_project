#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../general/ui.h"
#include "../general/sendReq.h"
#include "../general/structs.h"

bool loginUI(char *userName, char *password)
{
    char warning[50] = "";

    while (1)
    {
        cleanScreen();
        mprintf("Press ESC to exit\n");
        mprintf("Username: ");
        printf("\033[s");
        printf("\n");
        mprintf("Password: ");

        printf("\n\033[2B\033[5D");
        if (strlen(warning) > 0)
        {
            mprintf("%s\n", warning);
            strcpy(warning, "");
            strcpy(userName, "");
        }

        printf("\033[u");
        if (!getInputLine(userName, 50))
        {
            return false;
        }

        if (strlen(userName) == 0)
        {
            strcpy(warning, "Please write a valid username!");
        }
        else
        {
            break;
        }
    }

    while (1)
    {
        cleanScreen();
        mprintf("Press ESC to exit\n");
        mprintf("Username: %s\n", userName);

        mprintf("Password: ");
        printf("\033[s");

        printf("\n\033[2B\033[5D");
        if (strlen(warning) > 0)
        {
            mprintf("%s\n", warning);
            strcpy(warning, "");
            strcpy(password, "");
        }

        printf("\033[u");

        if (!getInputLine(password, sizeof(password)))
        {
            return false;
        }

        if (strlen(password) == 0)
        {
            strcpy(warning, "Please write a valid password!");
            continue;
        }

        break;
    }

    return true;
}

bool createAccountUI(char *userName, char *password, char *r_password)
{
    char warning[100] = "";

    while (1)
    {
        cleanScreen();
        mprintf("Press ESC to exit\n");
        mprintf("Username: ");
        printf("\033[s");

        printf("\n\033[2B\033[5D");
        if (strlen(warning) > 0)
        {
            mprintf("%s\n", warning);
            strcpy(warning, "");
            strcpy(userName, "");
        }

        printf("\033[u");
        if (!getInputLine(userName, 50))
        {
            return false;
        }

        if (strlen(userName) == 0)
        {
            strcpy(warning, "Please write a valid username!");
        }
        else
        {
            break;
        }
    }

    strcpy(warning, "");

    while (1)
    {
        cleanScreen();
        mprintf("Press ESC to exit\n");
        mprintf("Username: %s\n", userName);
        mprintf("Password: ");

        printf("\033[s");
        printf("\n\033[2B\033[5D");
        if (strlen(warning) > 0)
        {
            mprintf("%s\n", warning);
            strcpy(warning, "");
            strcpy(password, "");
            strcpy(r_password, "");
        }
        printf("\033[u");

        if (!getInputLine(password, 50))
        {
            return false;
        }

        if(strlen(password) == 0)
        {
            strcpy(warning, "Please write a valid password!");
            continue;
        }
        fflush(stdout);
        mprintf("Repeat your password: ");
        
        if (!getInputLine(r_password, 50))
        {
            return false;
        }

        if (strcmp(password, r_password) != 0)
        {
            strcpy(warning, "Please make sure your password is the same in both fields!");
        }
        else
        {
            break;
        }
    }

    return true;
}