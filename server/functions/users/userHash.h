#ifndef USERHASH_H
#define USERHASH_H

#define TABLE_SIZE 101

#include "../general/structs.h"

typedef struct Node{
    USER *user;
    struct Node* next;
} NODE;

unsigned int hash(const char *str);
bool hashSearch(const char *userName, USER **returnUser, NODE **hashTable);
bool hashInsert(USER user, NODE **hashTable);
void hashRemove(const char *userName, NODE **hashTable);

#endif