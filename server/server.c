#include <stdio.h>
#include <sys/socket.h>
#include "../config.h"

int create_socket(int *fd){
    *fd = socket(AF_INET,SOCK_STREAM,0);
    if(*fd == ERROR){
        return ERROR;
    }
    printf("Socket created successfully\n");
    return SUCCESS;
}