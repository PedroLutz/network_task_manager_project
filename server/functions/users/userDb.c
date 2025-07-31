#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../general/structs.h"
#include "../general/sync.h"
#include "userDb.h"

int fileUserFindLastId(void){
    pthread_rwlock_rdlock(&rwlockUsers);
    FILE *fp = fopen("files/lastId.bin", "rb");
    if(fp == NULL){
        pthread_rwlock_unlock(&rwlockUsers);
        return -1;
    }
    int id;
    fseek(fp, USER_ID_POS, SEEK_SET);
    fread(&id, sizeof(int), 1, fp);
    fclose(fp);
    pthread_rwlock_unlock(&rwlockUsers);
    return id;
}

void fileUserSetLastId(int id){
    pthread_rwlock_wrlock(&rwlockUsers);
    FILE *fp = fopen("files/lastId.bin", "r+b");
    if(fp == NULL){
        pthread_rwlock_unlock(&rwlockUsers);
        return;
    }
    fseek(fp, USER_ID_POS, SEEK_SET);
    fwrite(&id, sizeof(int), 1, fp);
    fclose(fp);
    pthread_rwlock_unlock(&rwlockUsers);
}

void fileUserFindLoop(USER *user, int id, unsigned short int *position, bool *found, FILE *fp){
    while (fread(user, sizeof(USER), 1, fp) != 0) {
        if (user->id == id) {
            *found = true;
            break;
        }
        (*position)++;
    }
}

bool fileUserFindPositionById(int id, unsigned short int *returnPosition)
{
    pthread_rwlock_rdlock(&rwlockUsers);
    FILE *fp;
    fp = fopen("files/users.bin", "rb");

    if (fp == NULL){
        pthread_rwlock_unlock(&rwlockUsers);
        return false;
    }
        
    USER *user = malloc(sizeof(USER));

    unsigned short int position = 0;
    bool found = false;
    fileUserFindLoop(user, id, &position, &found, fp);
    fclose(fp);
    pthread_rwlock_unlock(&rwlockUsers);

    if (!found){
        free(user);
        return found;
    } 

    free(user);
    *returnPosition = position;
    return found;
}

bool fileUserUpdate(USER *user, int id)
{
    pthread_rwlock_wrlock(&rwlockUsers);

    FILE *fp = fopen("files/users.bin", "r+b");
    if (fp == NULL){
        pthread_rwlock_unlock(&rwlockUsers);
        return false;
    }

    USER temp;
    unsigned short int position = 0;
    bool found = false;

    fileUserFindLoop(&temp, id, &position, &found, fp);

    if (found) {
        fseek(fp, position * sizeof(USER), SEEK_SET);
        fwrite(user, sizeof(USER), 1, fp);
    }

    fclose(fp);
    pthread_rwlock_unlock(&rwlockUsers);
    return found;
}


bool fileUserCreate(USER *user){
    pthread_rwlock_wrlock(&rwlockUsers);

    FILE *fp;
    fp = fopen("files/users.bin", "ab");
    if(fp == NULL){
        pthread_rwlock_unlock(&rwlockUsers);
        return false;
    }
    fwrite(user, sizeof(USER), 1, fp);
    fclose(fp);

    pthread_rwlock_unlock(&rwlockUsers);

    return true;
}

bool fileUserDeleteById(int id){
    pthread_rwlock_wrlock(&rwlockUsers);

    FILE *fpOriginal = fopen("files/users.bin", "rb");
    FILE *fpTemp = fopen("files/tempUsers.bin", "wb");

    if (!fpOriginal || !fpTemp) {
        puts("Error opening file.");
        if (fpOriginal) fclose(fpOriginal);
        if (fpTemp) fclose(fpTemp);
        pthread_rwlock_unlock(&rwlockUsers);
        return false;
    }

    USER user;
    int found = false;

    while (fread(&user, sizeof(USER), 1, fpOriginal) == 1) {
        if (user.id == id) {
            found = true; 
            continue;
        } 
        fwrite(&user, sizeof(USER), 1, fpTemp); 
    }

    fclose(fpOriginal);
    fclose(fpTemp);

    if (found) {
        remove("files/users.bin");
        rename("files/tempUsers.bin", "files/users.bin");
    } else {
        remove("files/tempUsers.bin");
    }

    pthread_rwlock_unlock(&rwlockUsers);

    return found;
}

char *fileUserGetAll(void){
    pthread_rwlock_rdlock(&rwlockUsers);
    FILE *fp;
    fp = fopen("files/users.bin", "rb");

    if (fp == NULL){
        pthread_rwlock_unlock(&rwlockUsers);
        return NULL;
    }
    
    size_t bufferSize = 1024;
    char *buffer = malloc(bufferSize);
    if(!buffer){
        pthread_rwlock_unlock(&rwlockUsers);
        return NULL;
    }
    buffer[0] = '\0';

    USER u;
    while(fread(&u, sizeof(USER), 1, fp) == 1){
        char temp[256];
        snprintf(temp, sizeof(temp), "%d;%s;%s\n", u.id, u.userName, u.password);

        if (strlen(buffer) + strlen(temp) + 1 > bufferSize) {
            bufferSize *= 2;
            buffer = realloc(buffer, bufferSize);
            if (!buffer) break;
        }

        strcat(buffer, temp);
    }

    fclose(fp);
    pthread_rwlock_unlock(&rwlockUsers);
    return buffer;
}