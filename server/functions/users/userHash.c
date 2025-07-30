#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "userHash.h"
#include "../general/structs.h"
#include "../general/sync.h"

unsigned int hash(const char *str) {
    unsigned int hash = 0;
    int c;
    while ((c = *str++)) {
        hash = (hash * 31 + c) % TABLE_SIZE;
    }
    return hash;
}

bool hashSearch(const char *userName, USER **returnUser, NODE **hashTable){
    unsigned int h = hash(userName);

    if(hashTable[h] == NULL){
        return false;
    }

    NODE *pNodeAux = hashTable[h];

    while(pNodeAux != NULL){
        if(strcmp(pNodeAux->user->userName, userName) == 0){
            if(returnUser != NULL){
                *returnUser = pNodeAux->user;
            }
            return true;
        }
        pNodeAux = pNodeAux->next;
    }

    return false;
}

bool hashInsert(USER user, NODE **hashTable){
    if(hashSearch(user.userName, NULL, hashTable)){
        return false;
    }

    USER *pUserAux = malloc(sizeof(USER));

    if(pUserAux == NULL){
        return false;
    }

    pUserAux->id = user.id;
    strcpy(pUserAux->userName, user.userName);
    strcpy(pUserAux->password, user.password);

    unsigned int h = hash(user.userName);

    NODE *newNode = malloc(sizeof(NODE));
    if(newNode == NULL){
        free(pUserAux);
        return false;
    }
    newNode->user = pUserAux;

    newNode->next = hashTable[h];
    hashTable[h] = newNode;

    return true;
}

void hashRemove(const char *userName, NODE **hashTable) {
    unsigned int h = hash(userName);
    NODE *cur = hashTable[h];
    NODE *prev = NULL;

    while (cur != NULL) {
        if (strcmp(cur->user->userName, userName) == 0) {
            if (prev == NULL) {
                hashTable[h] = cur->next;
            } else {
                prev->next = cur->next;
            }
            free(cur->user);
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}