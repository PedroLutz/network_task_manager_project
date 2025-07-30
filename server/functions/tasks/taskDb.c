#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../general/structs.h"
#include "../general/sync.h"
#include "taskDb.h"

int fileTaskFindLastId(void){
    pthread_rwlock_rdlock(&rwlockTasks);
    FILE *fp = fopen("files/lastId.bin", "rb");
    if(fp == NULL){
        pthread_rwlock_unlock(&rwlockTasks);
        return -1;
    }
    int id;
    fseek(fp, TASK_ID_POS, SEEK_SET);
    fread(&id, sizeof(int), 1, fp);
    fclose(fp);
    pthread_rwlock_unlock(&rwlockTasks);
    return id;
}

void fileTaskSetLastId(int id){
    pthread_rwlock_wrlock(&rwlockTasks);
    FILE *fp = fopen("files/lastId.bin", "r+b");
    if(fp == NULL){
        return;
    }
    fseek(fp, TASK_ID_POS, SEEK_SET);
    fwrite(&id, sizeof(int), 1, fp);
    fclose(fp);
    pthread_rwlock_unlock(&rwlockTasks);
}

void fileTaskFindLoop(TASK *task, int id, unsigned short int *position, bool *found, FILE *fp){
    while (fread(task, sizeof(TASK), 1, fp) != 0) {
        if (task->id == id) {
            *found = true;
            break;
        }
        (*position)++;
    }
}

bool fileTaskFindPositionById(int id, unsigned short int *returnPosition)
{
    pthread_rwlock_rdlock(&rwlockTasks);
    
    FILE *fp = fopen("files/tasks.bin", "rb");
    if (fp == NULL){
        pthread_rwlock_unlock(&rwlockTasks);
        return false;
    }

    TASK *task = malloc(sizeof(TASK));
    if (!task) {
        fclose(fp);
        pthread_rwlock_unlock(&rwlockTasks);
        return false;
    }

    unsigned short int position = 0;
    bool found = false;

    fileTaskFindLoop(task, id, &position, &found, fp);

    fclose(fp);
    pthread_rwlock_unlock(&rwlockTasks);

    if (!found){
        free(task);
        return found;
    }

    *returnPosition = position;
    free(task);
    return true;
}

char *fileTaskFindById(int id, char *returnBuffer)
{
    unsigned short int position;
    if (fileTaskFindPositionById(id, &position) == false) {
        return NULL;
    }

    pthread_rwlock_rdlock(&rwlockTasks);

    FILE *fp = fopen("files/tasks.bin", "rb");
    if (fp == NULL) {
        pthread_rwlock_unlock(&rwlockTasks);
        return NULL;
    }

    TASK *task = malloc(sizeof(TASK));
    if (!task) {
        fclose(fp);
        pthread_rwlock_unlock(&rwlockTasks);
        return NULL;
    }

    fseek(fp, sizeof(TASK) * position, SEEK_SET);
    fread(task, sizeof(TASK), 1, fp);
    fclose(fp);

    sprintf(returnBuffer, "%d;%d;%s;%d;%d\n",
            task->id,
            task->userId,
            task->description,
            task->status,
            task->priority);

    free(task);
    pthread_rwlock_unlock(&rwlockTasks);
    return returnBuffer;
}


char* fileTaskGetByUser(int userId){
    pthread_rwlock_rdlock(&rwlockTasks);
    FILE *fp;
    fp = fopen("files/tasks.bin", "rb");

    if (fp == NULL) {
        pthread_rwlock_unlock(&rwlockTasks);
        return NULL;
    }
        
    
    size_t bufferSize = 1024;
    char *buffer = malloc(bufferSize);
    if(!buffer){
        pthread_rwlock_unlock(&rwlockTasks);
        return NULL;
    } 
    buffer[0] = '\0';

    TASK t;
    while(fread(&t, sizeof(TASK), 1, fp) == 1){
        if(t.userId == userId){
            char temp[256];
            snprintf(temp, sizeof(temp), "%d;%d;%s;%d;%d\n", t.id, t.userId, t.description, t.status, t.priority);

            if (strlen(buffer) + strlen(temp) + 1 > bufferSize) {
                bufferSize *= 2;
                buffer = realloc(buffer, bufferSize);
                if (!buffer) break;
            }

            strcat(buffer, temp);
        }
    }

    fclose(fp);
    pthread_rwlock_unlock(&rwlockTasks);
    return buffer;
}

bool fileTaskCreate(TASK *task){
    pthread_rwlock_wrlock(&rwlockTasks);

    FILE *fp;
    fp = fopen("files/tasks.bin", "ab");
    if (fp == NULL) {
        pthread_rwlock_unlock(&rwlockTasks);
        return false;
    }
    
    fwrite(task, sizeof(TASK), 1, fp);
    fclose(fp);

    pthread_rwlock_unlock(&rwlockTasks);

    return true;
}

bool fileTaskUpdate(TASK *task, int id)
{
    pthread_rwlock_wrlock(&rwlockTasks);

    FILE *fp;
    fp = fopen("files/tasks.bin", "r+b");
    if (fp == NULL) {
        pthread_rwlock_unlock(&rwlockTasks);
        return false;
    }

    TASK temp;
    unsigned short int position = 0;
    bool found = false;

    fileTaskFindLoop(&temp, id, &position, &found, fp);

    if (found) {
        fseek(fp, position * sizeof(TASK), SEEK_SET);
        fwrite(task, sizeof(TASK), 1, fp);
    }

    fclose(fp);

    pthread_rwlock_unlock(&rwlockTasks);

    return true;
}

bool fileTaskDeleteById(int id){
    pthread_rwlock_wrlock(&rwlockTasks);

    FILE *fpOriginal = fopen("files/tasks.bin", "rb");
    FILE *fpTemp = fopen("files/tempTasks.bin", "wb");

    if (!fpOriginal || !fpTemp) {
        puts("Error opening file.");
        if (fpOriginal) fclose(fpOriginal);
        if (fpTemp) fclose(fpTemp);
        pthread_rwlock_unlock(&rwlockTasks);
        return false;
    }

    TASK task;
    int found = false;

    while (fread(&task, sizeof(TASK), 1, fpOriginal) == 1) {
        if (task.id == id) {
            found = true; 
            continue;
        }
        fwrite(&task, sizeof(TASK), 1, fpTemp); 
    }

    fclose(fpOriginal);
    fclose(fpTemp);

    if (found) {
        remove("files/tasks.bin");
        rename("files/tempTasks.bin", "files/tasks.bin");
        puts("User deleted successfully.");
    } else {
        remove("files/tempTasks.bin");
        puts("User not found.");
    }

    pthread_rwlock_unlock(&rwlockTasks);
    return found;
}

bool fileTaskDeleteByUser(int userId){
    pthread_rwlock_wrlock(&rwlockTasks);

    FILE *fpOriginal = fopen("files/tasks.bin", "rb");
    FILE *fpTemp = fopen("files/tempTasks.bin", "wb");

    if (!fpOriginal || !fpTemp) {
        puts("Error opening file.");
        if (fpOriginal) fclose(fpOriginal);
        if (fpTemp) fclose(fpTemp);
        pthread_rwlock_unlock(&rwlockTasks);
        return false;
    }

    TASK task;
    int found = false;

    while (fread(&task, sizeof(TASK), 1, fpOriginal) == 1) {
        if (task.userId != userId) {
            fwrite(&task, sizeof(TASK), 1, fpTemp); 
        } else {
            found = true;
        }
    }

    fclose(fpOriginal);
    fclose(fpTemp);

    if (found) {
        remove("files/tasks.bin");
        rename("files/tempTasks.bin", "files/tasks.bin");
        puts("User deleted successfully.");
    } else {
        remove("files/tempTasks.bin");
        puts("User not found.");
    }

    pthread_rwlock_unlock(&rwlockTasks);
    return found;
}