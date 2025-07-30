#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include "./functions/users/userCache.h"
#include "./functions/users/userReqHandler.h"
#include "./functions/tasks/taskReqHandler.h"

#define PORT 12345

int serverSocket;
volatile sig_atomic_t stopServer = 0;

void initLastIdFileIfNotExists() {
    FILE *fp = fopen("files/lastId.bin", "rb");
    if (fp == NULL) {
        fp = fopen("files/lastId.bin", "wb");
        int value = 0;
        fwrite(&value, sizeof(int), 1, fp); // user id
        fwrite(&value, sizeof(int), 1, fp); // task id
        fclose(fp);
    } else {
        fclose(fp);
    }
}

void handleSigint(int sig){
    printf("\n[!] CTRL+C detected, sig: %d, closing server...\n", sig);
    stopServer = 1;
    userCache_free();
    close(serverSocket);
}

void handleReq(char *table, char *req, char **response){
    if(strcmp(table, "users") == 0) handleUserReq(req, response);
    else if(strcmp(table, "tasks") == 0) handleTaskReq(req, response);
    else *response = strdup("invalid_table");
}

void *handleClient(void *arg){
    int clientSocket = *(int *)arg;
    free(arg);
    char buffer[1024];

    while(1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if(bytes <= 0) break;
        char *tableStr = strtok(buffer, "|");
        char *req = strtok(NULL, "");

        char table[10];
        sscanf(tableStr, "table:%9s", table);
        
        char *response = NULL;
        handleReq(table, req, &response);

        int responseLen = strlen(response);
        send(clientSocket, &responseLen, sizeof(int), 0);
        send(clientSocket, response, strlen(response), 0);
        free(response);
    }
    
    close(clientSocket);
    return NULL;
}

int main(){
    signal(SIGINT, handleSigint);
    initLastIdFileIfNotExists();
    userCache_init();

    int *newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size = sizeof(clientAddr);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 10);

    printf("Server listening in port %d...\n", PORT);

    while(!stopServer){
        newSocket = malloc(sizeof(int));
        *newSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);

        if(*newSocket < 0){
            if(stopServer){
                free(newSocket);
                break;
            } 
            free(newSocket);
            perror("accept");
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, handleClient, newSocket);
        pthread_detach(tid);
    }

    userCache_free();
    close(serverSocket);
    return 0;
}