#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "../general/structs.h"
#include "../general/modeParser.h"
#include "../tasks/taskDb.h"
#include "userCache.h"
#include "userDb.h"
#include "userReqHandler.h"

void handleUserReq(char *buffer, char **response)
{
    char *modeStr = strtok(buffer, "|");
    char *data = strtok(NULL, "");

    char operation[20];
    sscanf(modeStr, "mode:%19s", operation);

    int id = -1;
    char userName[50] = "";
    char password[50] = "";

    if (data != NULL)
    {
        char *token = strtok(data, ";");
        while (token != NULL)
        {
            if (sscanf(token, "id:%d", &id))
            {
            }
            else if (sscanf(token, "userName:%49[^;]", userName))
            {
            }
            else if (sscanf(token, "password:%49[^;]", password))
            {
            }
            token = strtok(NULL, ";");
        }
    }

    MODE parsedMode = parseMode(operation);

    USER user;
    user.id = id;
    strncpy(user.userName, userName, sizeof(user.userName));
    user.userName[sizeof(user.userName) - 1] = '\0';
    strncpy(user.password, password, sizeof(user.password));
    user.password[sizeof(user.password) - 1] = '\0';

    switch (parsedMode)
    {
    case MODE_CREATE:
    {
        int lastId = fileUserFindLastId();
        if (lastId < 0)
            lastId = 0;
        user.id = lastId + 1;

        if (userCache_search(user.userName))
        {
            *response = strdup("userName_exists");
            break;
        }

        bool created = fileUserCreate(&user);
        bool inserted = userCache_insert(user);

        if (!created || !inserted)
        {
            *response = strdup("create_fail");
            break;
        }

        fileUserSetLastId(user.id);
        *response = strdup("create_success");
        break;
    }
    case MODE_UPDATE:
    {
        if (userCache_search(user.userName))
        {
            *response = strdup("userName_exists");
            break;
        }

        if (fileUserUpdate(&user, id))
        {
            userCache_update(user);
            *response = strdup("update_success");
        }
        else
        {
            *response = strdup("update_fail");
        }
        break;
    }
    case MODE_DELETE:
        char res1[50];
        if (fileUserDeleteById(id))
        {
            strcpy(res1, "delete_success;");
            userCache_remove(id);
        }
        else
        {
            strcpy(res1, "delete_fail;");
        }
        const char *res2 = fileTaskDeleteByUser(id) ? "delete_tasks_success" : "delete_tasks_fail";

        size_t len = strlen(res1) + strlen(res2) + 1;
        *response = malloc(len);
        if (*response)
            snprintf(*response, len, "%s%s", res1, res2);
        break;
    case MODE_GET:
        userCache_checkLogin(user, response);
        break;
    default:
    {
        puts("Invalid request");
        *response = strdup("invalid_request");
    }
    }
}
