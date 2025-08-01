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

static bool compareTasks(TASK *t1, TASK *t2, ORDER_MODE orderMode)
{
    if (orderMode == ID)
    {
        return t1->id < t2->id;
    }
    else if (orderMode == NAME)
    {
        return strcmp(t1->description, t2->description) < 0;
    }
    else if (orderMode == PRIORITY)
    {
        if (t1->priority == t2->priority)
        {
            return strcmp(t1->description, t2->description) < 0;
        }
        else
        {
            return t1->priority > t2->priority;
        }
    }
    return false;
}

void treeInsert(NODE **root, TASK *task, ORDER_MODE orderMode)
{
    NODE *node = createNode(task);
    NODE *y = NULL;
    NODE *x = *root;

    while (x != NULL)
    {
        y = x;

        bool isNodeSmallerThanX = compareTasks(node->task, x->task, orderMode);

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
        bool isNodeSmallerThanY = compareTasks(node->task, y->task, orderMode);

        if (isNodeSmallerThanY)
            y->left = node;
        else
            y->right = node;
    }

    adjustInsertion(root, node);
}

bool treeSearchById(NODE *root, NODE **found, int id)
{
    if (root == NULL)
    {
        *found = NULL;
        return false;
    }

    if (root->task->id == id)
    {
        *found = root;
        return true;
    }
    else if (root->task->id > id)
    {
        return treeSearchById(root->left, found, id);
    }
    else
    {
        return treeSearchById(root->right, found, id);
    }
}

static NODE *sibling(NODE *node)
{
    if (node->parent == NULL)
        return NULL;
    return (node == node->parent->left) ? node->parent->right : node->parent->left;
}

static bool isBlack(NODE *node)
{
    return node == NULL || node->color == BLACK;
}

static bool isRed(NODE *node)
{
    return node != NULL && node->color == RED;
}

static void swapColors(NODE *a, NODE *b)
{
    COLOR temp = a->color;
    a->color = b->color;
    b->color = temp;
}

static NODE *correctDoubleBlack(NODE *root, NODE *node)
{
    if (node == NULL)
        return root;

    NODE *siblingNode = sibling(node);
    if (siblingNode == NULL)
        return root;

    if (isRed(siblingNode))
    {
        swapColors(node->parent, siblingNode);
        if (siblingNode == node->parent->right)
            rotateLeft(&root, node->parent);
        else
            rotateRight(&root, node->parent);
        siblingNode = sibling(node);
    }

    if (isBlack(siblingNode->left) && isBlack(siblingNode->right))
    {
        siblingNode->color = RED;
        if (node->parent->color == RED)
        {
            node->parent->color = BLACK;
        }
        else
        {
            root = correctDoubleBlack(root, node->parent);
        }
    }

    else
    {
        if (siblingNode == node->parent->right)
        {
            if (isBlack(siblingNode->right))
            {
                swapColors(siblingNode, siblingNode->left);
                rotateRight(&root, siblingNode);
                siblingNode = node->parent->right;
            }
            swapColors(node->parent, siblingNode);
            siblingNode->right->color = BLACK;
            rotateLeft(&root, node->parent);
        }
        else
        {
            if (isBlack(siblingNode->left))
            {
                swapColors(siblingNode, siblingNode->right);
                rotateLeft(&root, siblingNode);
                siblingNode = node->parent->left;
            }
            swapColors(node->parent, siblingNode);
            siblingNode->left->color = BLACK;
            rotateRight(&root, node->parent);
        }
    }

    return root;
}

static NODE *minimumNode(NODE *node)
{
    while (node->left != NULL)
        node = node->left;
    return node;
}

NODE *treeRemoveNode(NODE *root, NODE *node)
{

    if (node->left && node->right)
    {
        NODE *successor = minimumNode(node->right);

        TASK *temp = node->task;
        node->task = successor->task;
        successor->task = temp;

        root = treeRemoveNode(root, successor);
        return root;
    }
    
    NODE *child = (node->left != NULL) ? node->left : node->right;

    if (node->parent == NULL)
    {
        root = child;
        if (child != NULL)
            child->parent = NULL;
        free(node);
        if (root != NULL)
            root->color = BLACK;
        return root;
    }

    if (isRed(node) || (child != NULL && isRed(child)))
    {
        if (node == node->parent->left)
            node->parent->left = child;
        else
            node->parent->right = child;
        if (child != NULL)
        {
            child->parent = node->parent;
            child->color = RED;
        }
        free(node);
        return root;
    }

    NODE *parent = node->parent;
    if (node == parent->left)
        parent->left = NULL;
    else
        parent->right = NULL;
    free(node);

    root = correctDoubleBlack(root, parent);
    return root;
}

void printTree(NODE *root)
{
    if (root == NULL)
        return;
    printTree(root->left);
    TASK *task = root->task;
    printf("%d %d %s\n", task->id, task->priority, task->description);
    printTree(root->right);
}

void treeFree(NODE *root)
{
    if (root == NULL)
        return;
    treeFree(root->left);
    treeFree(root->right);
    free(root->task);
    free(root);
}

void treeCountNodes(NODE *root, int *counter)
{
    if (root == NULL)
        return;

    treeCountNodes(root->left, counter);
    (*counter)++;
    treeCountNodes(root->right, counter);
}

NODE *treeClone(NODE *root)
{
    if (root == NULL)
        return NULL;

    NODE *newNode = malloc(sizeof(NODE));
    if (!newNode)
        return NULL;

    newNode->task = malloc(sizeof(TASK));
    newNode->task->id = root->task->id;
    strcpy(newNode->task->description, root->task->description);
    newNode->task->priority = root->task->priority;
    newNode->task->status = root->task->status;
    newNode->task->userId = root->task->userId;

    newNode->color = root->color;
    newNode->left = treeClone(root->left);
    newNode->right = treeClone(root->right);
    newNode->parent = NULL;

    if (newNode->left)
        newNode->left->parent = newNode;
    if (newNode->right)
        newNode->right->parent = newNode;

    return newNode;
}

NODE *treeGetNodeByIndex(NODE *root, int *currentIndex, int targetIndex)
{
    if (root == NULL)
        return NULL;

    NODE *left = treeGetNodeByIndex(root->left, currentIndex, targetIndex);
    if (left != NULL)
        return left;

    (*currentIndex)++;
    if (*currentIndex == targetIndex)
        return root;

    return treeGetNodeByIndex(root->right, currentIndex, targetIndex);
}