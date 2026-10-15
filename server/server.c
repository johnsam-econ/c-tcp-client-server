#include <stdio.h>
#include <sys/socket.h>
#include "../config.h"
#include <netinet/in.h>

int create_socket(int *fd){
    *fd = socket(AF_INET,SOCK_STREAM,0);
    if(*fd == ERROR){
        perror("Error in binding the socket");
        return ERROR;
    }
    printf("Socket created successfully\n");
    return SUCCESS;
}

int bind_socket(int fd,struct sockaddr_in addr,socklen_t addrlen){
    if(bind(fd,(struct sockaddr*) &addr,addrlen) == ERROR){
        perror("Error in binding the socket");
        return ERROR;
    }
    printf("Binding created successfully\n");
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

int accept_socket(int *fd,int *new_fd,struct sockaddr_in* addr,socklen_t* addrlen){
    *new_fd = accept(*fd,(struct sockaddr*)addr,addrlen);
    if(*new_fd == ERROR){
        perror("Error in accepting the new socket");
        return ERROR;
    }
    printf("Client Connected sucessfully...\n");
    return SUCCESS;
}

void addr_defn(struct sockaddr_in *addr){
    (*addr).sin_family = AF_INET;
    (*addr).sin_port = htons(PORT);
    (*addr).sin_addr.s_addr = INADDR_ANY;
}