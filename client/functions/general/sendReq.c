#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include "sendReq.h"

#define PORT 12345

char *buildReq(char *table, char *mode, char *req, char **returnReq){
    if(strlen(req) > 231){
        return NULL;
    }

    char temp[256];
    snprintf(temp, sizeof(temp), "table:%s|mode:%s|%s", table, mode, req);

    *returnReq = malloc(strlen(temp) + 1);
    if(*returnReq == NULL) return NULL;

    strcpy(*returnReq, temp);
    return *returnReq;
}

static int recvAll(int sock, void *buf, int len) {
    int total = 0;
    while (total < len) {
        int n = recv(sock, (char *)buf + total, len - total, 0);
        if (n <= 0) return -1;
        total += n;
    }
    return total;
}

bool sendReq(char *req, char **res){
    int sock;
    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        close(sock);
        return false;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(sock,(struct sockaddr*)&server, sizeof(server)) < 0){
        close(sock);
        return false;
    }

    send(sock, req, strlen(req), 0);

    int responseLen;    
    if(recv(sock, &responseLen, sizeof(int), 0) <= 0){
        close(sock);
        return false;
    }

    *res = malloc(responseLen + 1);
    if(!*res || recvAll(sock, *res, responseLen) != responseLen){
        close(sock);
        return false;
    }
    
    (*res)[responseLen] = '\0';
    close(sock);
    return true;
}