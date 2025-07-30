#ifndef STRUCTS_H
#define STRUCTS_H

#define MAX_TASKS 100

typedef enum Status {START = 1, EXECUTING, FINISHED} STATUS;

typedef struct User{
    int id;
    char userName[50];
    char password[50];
} USER;

typedef struct Task{
    int id;
    int userId;
    char description[100];
    STATUS status;
    int priority;
} TASK;

#endif