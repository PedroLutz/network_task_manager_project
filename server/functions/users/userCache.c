#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "userCache.h"
#include "userDb.h"
#include "userHash.h"
#include "../general/structs.h"

static NODE *hashTable[TABLE_SIZE];

void userCache_init(void){
    for(int i = 0; i < TABLE_SIZE; i++){
        hashTable[i] = NULL;
    }

    char *data = fileUserGetAll();
    if(data == NULL){
        return;
    }

    char *saveptr1;
    char *line = strtok_r(data, "\n", &saveptr1);
    while(line != NULL){
        if(strlen(line) == 0) { 
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }

        USER u;
        char lineCopy[256];
        strncpy(lineCopy, line, sizeof(lineCopy));
        lineCopy[sizeof(lineCopy) - 1] = '\0';

        char *saveptr2;
        char *part = strtok_r(lineCopy, ";", &saveptr2);
        if(part == NULL) {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        u.id = atoi(part);

        part = strtok_r(NULL, ";", &saveptr2);
        if(part == NULL) {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        strcpy(u.userName, part);

        part = strtok_r(NULL, ";", &saveptr2);
        if(part == NULL) {
            line = strtok_r(NULL, "\n", &saveptr1);
            continue;
        }
        strcpy(u.password, part);

        hashInsert(u, hashTable);

        line = strtok_r(NULL, "\n", &saveptr1);
    }

    free(data);
}


bool userCache_insert(USER user){
    return hashInsert(user, hashTable);
}

bool userCache_search(const char *userName){
    return hashSearch(userName, NULL, hashTable);
}

void userCache_update(USER newData){
    bool found = false;
    for(int i = 0; i < TABLE_SIZE; i++){
        NODE *pNodeAux = hashTable[i];
        while(pNodeAux != NULL){
            if(pNodeAux->user->id == newData.id){
                hashRemove(pNodeAux->user->userName, hashTable);
                if(hashInsert(newData, hashTable) == false){
                    return;
                }
                found = true;
                break;
            }
        }
        if(found) break;
    }
}

void userCache_checkLogin(USER user, char **res){
    USER *foundUser = NULL;
    if(!hashSearch(user.userName, &foundUser, hashTable)){
        *res = strdup("userName_notFound");
        return;
    }
    if(strcmp(user.password, foundUser->password) != 0){
        *res = strdup("password_wrong");
        return;
    }
    char temp[128];
    sprintf(temp, "password_ok|id:%d", foundUser->id);
    *res = strdup(temp);
}

void userCache_free(void){
    for(int i = 0; i < TABLE_SIZE; i++){
        NODE *pNodeCur = hashTable[i];
        while(pNodeCur != NULL){
            NODE *pTemp = pNodeCur->next;
            free(pNodeCur->user);
            free(pNodeCur);
            pNodeCur = pTemp;
        }
        hashTable[i] = NULL;
    }
}