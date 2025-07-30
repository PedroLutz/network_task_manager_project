#ifndef TASKRBTREE_H
#define TASKRBTREE_H

#include "../general/structs.h"
#include <stdbool.h>

typedef enum Color {RED, BLACK} COLOR;

typedef enum OrderMode {ID, NAME} ORDER_MODE;

typedef struct Node{
    TASK *task;
    COLOR color;
    struct Node *left, *right, *parent;
} NODE;

void treeInsert(NODE **root, TASK *task, ORDER_MODE orderMode);
bool treeSearchById(NODE *root, NODE** found, int id);
NODE* treeRemoveNode(NODE* raiz, NODE* node);
void treeFree(NODE* root);

#endif