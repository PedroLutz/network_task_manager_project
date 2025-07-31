#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../general/structs.h"
#include "../general/ui.h"
#include "../general/sendReq.h"
#include "taskRbTree.h"
#include "taskData.h"
#include "taskUI.h"

bool taskGetUserTasks(int userId, NODE **root, char **err)
{
    char reqData[20];
    snprintf(reqData, sizeof(reqData), "userId:%d", userId);
    char *req;

    if (!buildReq(TABLE_TASKS, MODE_GET, reqData, &req))
    {
        *err = strdup("Error while generating request!");
        return false;
    }

    char *res;
    if (!sendReq(req, &res))
    {
        *err = strdup("Error while sending the request!");
        free(req);
        return false;
    }

    if (strcmp(res, "get_fail") == 0)
    {
        *err = strdup("Error while receiving the data!");
        free(req);
        free(res);
        return false;
    }

    if (res[0] == '\0')
    {
        free(req);
        free(res);
        return true;
    }

    char *saveptr1;
    char *line = strtok_r(res, "\n", &saveptr1);
    while (line != NULL)
    {
        if (strlen(line) == 0)
        {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }

        TASK *t = malloc(sizeof(TASK));
        char lineCopy[256];
        strncpy(lineCopy, line, sizeof(lineCopy));
        lineCopy[sizeof(lineCopy) - 1] = '\0';

        char *saveptr2;
        char *part = strtok_r(lineCopy, ";", &saveptr2);
        if (part == NULL)
        {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        t->id = atoi(part);

        part = strtok_r(NULL, ";", &saveptr2);
        if (part == NULL)
        {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        t->userId = atoi(part);

        part = strtok_r(NULL, ";", &saveptr2);
        if (part == NULL)
        {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        strcpy(t->description, part);

        part = strtok_r(NULL, ";", &saveptr2);
        if (part == NULL)
        {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        t->status = atoi(part);

        part = strtok_r(NULL, ";", &saveptr2);
        if (part == NULL)
        {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        t->priority = atoi(part);

        treeInsert(root, t, ID);

        line = strtok_r(NULL, "\n", &saveptr1);
    }

    free(req);
    free(res);
    return true;
}

bool createTask(int userID, NODE **root, char **msg)
{
    TASK *task = malloc(sizeof(TASK));

    if (!createTaskUI(task))
    {
        return false;
    }

    char reqData[512];
    snprintf(reqData, sizeof(reqData),
             "userId:%d;description:%s;status:%d;priority:%d",
             userID, task->description, START, task->priority);
    char *req;

    if (!buildReq(TABLE_TASKS, MODE_CREATE, reqData, &req))
    {
        *msg = strdup("Error while generating request!");
        return true;
    }

    char *res;
    if (!sendReq(req, &res))
    {
        *msg = strdup("Error while sending the request!");
        free(req);
        return true;
    }

    if (strcmp(res, "create_fail") == 0)
    {
        *msg = strdup("Error while creating the task!");
        free(req);
        free(res);
        return true;
    }

    char *token;
    token = strtok(res, "|");
    if (strcmp(token, "create_success") == 0)
    {
        token = strtok(NULL, "\0");
        int id;
        sscanf(token, "id:%d", &id);
        task->id = id;
        task->status = START;
        task->userId = userID;

        *msg = strdup("Task created successfully!");
        treeInsert(root, task, ID);
        free(req);
        free(res);
        return true;
    }

    *msg = strdup("Error while receiving server response!");
    free(req);
    free(res);
    return true;
}

static void createSubTree(NODE *baseRoot, NODE **targetRoot, STATUS status, ORDER_MODE orderMode)
{
    if (baseRoot == NULL)
    {
        return;
    }

    createSubTree(baseRoot->left, targetRoot, status, orderMode);

    bool isTargetStatus = status < 4 ? baseRoot->task->status == status : true;
    if (isTargetStatus)
    {
        treeInsert(targetRoot, baseRoot->task, orderMode);
    }

    createSubTree(baseRoot->right, targetRoot, status, orderMode);
}

static bool sendUpdateReq(char *reqData, char **msg)
{
    char *req;

    if (!buildReq(TABLE_TASKS, MODE_UPDATE, reqData, &req))
    {
        *msg = strdup("Error while generating request!");
        return false;
    }

    char *res;
    if (!sendReq(req, &res))
    {
        *msg = strdup("Error while sending the request!");
        free(req);
        return false;
    }

    if (strcmp(res, "update_fail") == 0)
    {
        *msg = strdup("Error while updating the task!");
        free(req);
        free(res);
        return false;
    }

    if (strcmp(res, "update_success") == 0)
    {
        free(req);
        free(res);
        return true;
    }

    *msg = strdup("Error while receiving server response!");
    free(req);
    free(res);
    return false;
}

static bool updateStatus(TASK *task, char **msg, STATUS *finalStatus)
{
    *finalStatus = task->status;

    int option = generateMenu(3,
                              "Start task",
                              "Finish task",
                              "Go back");

    if (option == 3)
    {
        return true;
    }

    char reqData[512];
    if (option == 1)
    {
        snprintf(reqData, sizeof(reqData),
                 "id:%d;userId:%d;description:%s;status:%d;priority:%d",
                 task->id, task->userId, task->description, EXECUTING, task->priority);
        *finalStatus = EXECUTING;
    }
    else
    {
        snprintf(reqData, sizeof(reqData),
                 "id:%d;userId:%d;description:%s;status:%d;priority:%d",
                 task->id, task->userId, task->description, FINISHED, task->priority);
        *finalStatus = FINISHED;
    }

    bool success = sendUpdateReq(reqData, msg);

    task->status = *finalStatus;

    return success;
}

static bool updateTask(TASK *task, char **msg)
{
    if (!createTaskUI(task))
    {
        return false;
    }

    char reqData[512];
    snprintf(reqData, sizeof(reqData),
             "id:%d;userId:%d;description:%s;status:%d;priority:%d",
             task->id, task->userId, task->description, task->status, task->priority);

    return sendUpdateReq(reqData, msg);
}

static bool deleteTask(TASK *task, char **msg)
{
    int option = 1;
    printf("\033[H");
    cleanScreen();
    mprintf("Are you sure you want to delete this task?\n\n");
    printf("\033[s");
    mprintf("Yes\n");
    mprintf("No\n");
    printf("\033[2B");
    mprintf("Use arrow up or down to select the option");
    printf("\033[u");
    printf("\033[32m  ->\033[0m");

    disableEcho();

    while (1)
    {
        char c = getchar();
        printf("\033[5D");
        if (c == '\033')
        {
            char c2 = getchar();
            char c3 = getchar();
            if (c2 == '[')
            {
                if (c3 == 'A')
                {
                    if (option > 1)
                    {
                        option--;
                        printf("     \r\033[1A\033[32m  ->\033[0m");
                    }
                }
                else if (c3 == 'B')
                {
                    if (option < 2)
                    {
                        option++;
                        printf("     \r\033[1B\033[32m  ->\033[0m");
                    }
                }
            }
        }
        else if (c == '\n')
        {
            enableEcho();
            break;
        }
    }

    if (option == 1)
    {
        char reqData[512];
        snprintf(reqData, sizeof(reqData),
                 "id:%d",
                 task->id);

        char *req;

        if (!buildReq(TABLE_TASKS, MODE_DELETE, reqData, &req))
        {
            *msg = strdup("Error while generating request!");
            return false;
        }

        char *res;
        if (!sendReq(req, &res))
        {
            *msg = strdup("Error while sending the request!");
            free(req);
            return false;
        }

        if (strcmp(res, "delete_fail") == 0)
        {
            *msg = strdup("Error while deleting the task!");
            free(req);
            free(res);
            return false;
        }

        if (strcmp(res, "delete_success") == 0)
        {
            free(req);
            free(res);
            return true;
        }

        *msg = strdup("Error while receiving server response!");
        free(req);
        free(res);
        return false;
    }

    return false;
}

void viewAllTasks(NODE **root)
{
    int status = generateMenu(5,
                              "See tasks to begin",
                              "See tasks in execution",
                              "See complete tasks",
                              "See all tasks",
                              "Go back");

    if (status == 5)
    {
        return;
    }

    int orderMode = generateMenu(4,
                                 "Order by ID",
                                 "Order by description",
                                 "Order by priority",
                                 "Go back");

    if (orderMode == 4)
    {
        return;
    }

    NODE *orderedTree = NULL;
    if (status == 4 && orderMode == ID)
    {
        orderedTree = treeClone(*root);
    }
    else
    {
        createSubTree(*root, &orderedTree, status, orderMode);
    }

    while (1)
    {
        int numNodes = 0;
        treeCountNodes(orderedTree, &numNodes);
        int task = generateTaskMenuFromRB(orderedTree, numNodes);

        if (task == 0)
        {
            treeFree(orderedTree);
            return;
        }

        int currentIndex = 0;
        NODE *selectedNode = treeGetNodeByIndex(orderedTree, &currentIndex, task);
        TASK *selectedTask = selectedNode->task;

        int option = generateMenu(4,
                                  "Update status",
                                  "Update task data",
                                  "Delete task",
                                  "Go back");

        char *msg = NULL;

        switch (option)
        {
        case 1:
        {
            STATUS finalStatus;
            if (!updateStatus(selectedTask, &msg, &finalStatus))
            {
                printMessage(msg);
                free(msg);
            }
            else
            {
                if (status != 4)
                {
                    if ((int)finalStatus != status)
                    {
                        orderedTree = treeRemoveNode(orderedTree, selectedNode);
                    }
                }
            }
            break;
        }

        case 2:
        {
            TASK *taskCopy = malloc(sizeof(TASK));
            if (!taskCopy)
                break; 

            *taskCopy = *selectedTask; 

            if (!updateTask(taskCopy, &msg))
            {
                if (msg != NULL)
                {
                    printMessage(msg);
                    free(msg);
                }
                break;
            }

            orderedTree = treeRemoveNode(orderedTree, selectedNode);

            NODE *originalNode = NULL;
            treeSearchById(*root, &originalNode, selectedTask->id);
            if (originalNode != NULL)
            {
                free(selectedTask);
                *root = treeRemoveNode(*root, originalNode);      
            }

            treeInsert(&orderedTree, taskCopy, orderMode);
            treeInsert(root, taskCopy, orderMode);

            selectedTask = taskCopy; 
            break;
        }

        case 3:
        {
            if (deleteTask(selectedTask, &msg))
            {
                orderedTree = treeRemoveNode(orderedTree, selectedNode);

                NODE *originalNode = NULL;
                treeSearchById(*root, &originalNode, selectedTask->id);
                *root = treeRemoveNode(*root, originalNode);
                free(selectedTask);
            }
            else
            {
                if (msg != NULL)
                {
                    printMessage(msg);
                    free(msg);
                }
            }
            break;
        }

        case 4:
            break;
        }
    }
    treeFree(orderedTree);
}