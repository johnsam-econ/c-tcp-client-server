#include <stdio.h>
#include <sys/socket.h>
#include "../config.h"

int create_socket(int *fd){
    *fd = socket(AF_INET,SOCK_STREAM,0);
    if(*fd == ERROR){
        perror("Error in binding the socket");
        return ERROR;
    }
    printf("Socket created successfully\n");
    return SUCCESS;
}

int listen_socket(int *fd){
    if(listen(*fd,MAX_QUEUE_LEN) == -1){
        perror("Error in listening the Socket");
        return ERROR;
    }
    printf("listen done successfully\n");
    return SUCCESS;
}