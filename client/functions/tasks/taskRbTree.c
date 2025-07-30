#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "taskRbTree.h"
#include "../general/structs.h"
#include <string.h>

static NODE *createNode(TASK *task)
{
    NODE *newNode = malloc(sizeof(NODE));
    newNode->task = task;
    newNode->color = RED;
    newNode->left = newNode->right = newNode->parent = NULL;
    return newNode;
}

static void rotateLeft(NODE **root, NODE *x)
{
    NODE *RightOfX = x->right;
    x->right = RightOfX->left;
    if (RightOfX->left != NULL)
        RightOfX->left->parent = x;

    RightOfX->parent = x->parent;

    if (x->parent == NULL)
        *root = RightOfX;
    else if (x == x->parent->left)
        x->parent->left = RightOfX;
    else
        x->parent->right = RightOfX;

    RightOfX->left = x;
    x->parent = RightOfX;
}

static void rotateRight(NODE **root, NODE *y)
{
    NODE *leftOfY = y->left;
    y->left = leftOfY->right;
    if (leftOfY->right != NULL)
        leftOfY->right->parent = y;

    leftOfY->parent = y->parent;

    if (y->parent == NULL)
        *root = leftOfY;
    else if (y == y->parent->right)
        y->parent->right = leftOfY;
    else
        y->parent->left = leftOfY;

    leftOfY->right = y;
    y->parent = leftOfY;
}

static void adjustInsertion(NODE **root, NODE *node)
{
    while (node != *root && node->parent->color == RED)
    {
        NODE *parent = node->parent;
        NODE *gparent = parent->parent;

        if (parent == gparent->left)
        {
            NODE *uncle = gparent->right;

            if (uncle != NULL && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                gparent->color = RED;
                node = gparent;
            }
            else
            {
                if (node == parent->right)
                {
                    node = parent;
                    rotateLeft(root, node);
                    parent = node->parent;
                    gparent = parent->parent;
                }
                parent->color = BLACK;
                gparent->color = RED;
                rotateRight(root, gparent);
            }
        }
        else
        {
            NODE *uncle = gparent->left;

            if (uncle != NULL && uncle->color == RED)
            {
                parent->color = BLACK;
                uncle->color = BLACK;
                gparent->color = RED;
                node = gparent;
            }
            else
            {
                if (node == parent->left)
                {
                    node = parent;
                    rotateRight(root, node);
                    parent = node->parent;
                    gparent = parent->parent;
                }
                parent->color = BLACK;
                gparent->color = RED;
                rotateLeft(root, gparent);
            }
        }
    }
    (*root)->color = BLACK;
}

void treeInsert(NODE **root, TASK *task, ORDER_MODE orderMode)
{
    NODE *node = createNode(task);
    NODE *y = NULL;
    NODE *x = *root;

    while (x != NULL)
    {
        y = x;

        bool isNodeSmallerThanX = orderMode == ID ? node->task->id < x->task->id : strcmp(node->task->description, x->task->description) < 0;

        if (isNodeSmallerThanX)
            x = x->left;
        else
            x = x->right;
    }

    node->parent = y;

    if (y == NULL)
    {
        *root = node;
    }
    else
    {
        bool isNodeSmallerThanY = orderMode == ID ? 
        node->task->id < y->task->id : 
        strcmp(node->task->description, y->task->description) < 0;

        if (isNodeSmallerThanY)
            y->left = node;
        else
            y->right = node;
    }

    adjustInsertion(root, node);
}

bool treeSearchById(NODE *root, NODE** found, int id){
    if(root == NULL){
        *found = NULL;
        return false;
    } 

    if(root->task->id == id){
        *found = root;
        return true;
    } else if (root->task->id > id)
    {
        return treeSearchById(root->left, found, id);
    } else
    {
        return treeSearchById(root->right, found, id);
    }
}

static NODE* sibling(NODE* node) {
    if (node->parent == NULL) return NULL;
    return (node == node->parent->left) ? node->parent->right : node->parent->left;
}

static bool isBlack(NODE* node) {
    return node == NULL || node->color == BLACK;
}

static bool isRed(NODE* node) {
    return node != NULL && node->color == RED;
}

static void swapColors(NODE* a, NODE* b) {
    COLOR temp = a->color;
    a->color = b->color;
    b->color = temp;
}

static NODE* correctDoubleBlack(NODE* raiz, NODE* node) {
    if (node == NULL) return raiz;

    NODE* siblingNode = sibling(node);

    if (isRed(siblingNode)) {
        swapColors(node->parent, siblingNode);
        if (siblingNode == node->parent->right)
            rotateLeft(&raiz, node->parent);
        else
            rotateRight(&raiz, node->parent);
        siblingNode = sibling(node);
    }

    if (isBlack(siblingNode->left) && isBlack(siblingNode->right)) {
        siblingNode->color = RED;
        if (node->parent->color == RED) {
            node->parent->color = BLACK;
        } else {
            raiz = correctDoubleBlack(raiz, node->parent);
        }
    }

    else {
        if (siblingNode == node->parent->right) {
            if (isBlack(siblingNode->right)) {
                swapColors(siblingNode, siblingNode->left);
                rotateRight(&raiz, siblingNode);
                siblingNode = node->parent->right;
            }
            swapColors(node->parent, siblingNode);
            siblingNode->right->color = BLACK;
            rotateLeft(&raiz, node->parent);
        } else {
            if (isBlack(siblingNode->left)) {
                swapColors(siblingNode, siblingNode->right);
                rotateLeft(&raiz, siblingNode);
                siblingNode = node->parent->left;
            }
            swapColors(node->parent, siblingNode);
            siblingNode->left->color = BLACK;
            rotateRight(&raiz, node->parent);
        }
    }

    return raiz;
}

NODE* treeRemoveNode(NODE* raiz, NODE* node) {
    NODE* child = (node->left != NULL) ? node->left : node->right;

    if (node->parent == NULL) {
        raiz = child;
        if (child != NULL) child->parent = NULL;
        free(node);
        if (raiz != NULL) raiz->color = BLACK;
        return raiz;
    }

    if (isRed(node) || isRed(child)) {
        if (node == node->parent->left) node->parent->left = child;
        else node->parent->right = child;
        if (child != NULL) {
            child->parent = node->parent;
            child->color = RED;
        }
        free(node);
        return raiz;
    }

    NODE* parent = node->parent;
    if (node == parent->left) parent->left = NULL;
    else parent->right = NULL;
    free(node);

    raiz = correctDoubleBlack(raiz, parent);
    return raiz;
}

void treeFree(NODE* root) {
    if (root == NULL) return;
    treeFree(root->left);
    treeFree(root->right);
    free(root);
}