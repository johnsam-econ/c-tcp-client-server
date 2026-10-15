#include <stdio.h>
#include <sys/socket.h>
#include "../config.h"
#include <netinet/in.h>
#include <string.h>

// Create an IPv4 TCP socket; the caller is responsible for closing it.
int create_socket(int *fd){
    *fd = socket(AF_INET,SOCK_STREAM,0);
    if(*fd == ERROR){
        perror("Error in binding the socket");
        return ERROR;
    }
    printf("Socket created successfully\n");
    return SUCCESS;
}

// Bind the listening socket to the address and port configured in config.h.
int bind_socket(int fd,struct sockaddr_in addr,socklen_t addrlen){
    if(bind(fd,(struct sockaddr*) &addr,addrlen) == ERROR){
        perror("Error in binding the socket");
        return ERROR;
    }
    printf("Binding created successfully\n");
    return SUCCESS;
}

// Start listening; MAX_QUEUE_LEN limits pending connections, not active clients.
int listen_socket(int *fd){
    if(listen(*fd,MAX_QUEUE_LEN) == -1){
        perror("Error in listening the Socket");
        return ERROR;
    }
    printf("listen done successfully\n");
    return SUCCESS;
}

// Accept one client and return its connected socket in new_fd.
int accept_socket(int *fd,int *new_fd,struct sockaddr_in* addr,socklen_t* addrlen){
    *new_fd = accept(*fd,(struct sockaddr*)addr,addrlen);
    if(*new_fd == ERROR){
        perror("Error in accepting the new socket");
        return ERROR;
    }
    printf("Client Connected sucessfully...\n");
    return SUCCESS;
}

// Prepare the IPv4 endpoint to listen on every local interface.
void addr_defn(struct sockaddr_in *addr){
    (*addr).sin_family = AF_INET;
    (*addr).sin_port = htons(PORT);
    (*addr).sin_addr.s_addr = INADDR_ANY;
}

// Return recv's byte count, 0 on disconnect, or ERROR on failure.
int server_receive(int fd,char *buffer,int len){
    int status = recv(fd,buffer,len,0);

    if(status == -1){
        perror("Error in Reading the data from the server");
    }
    else if(status == 0){
        printf("Client Disconnected..\n");
    }
    else{
        // recv does not append a string terminator; data is a byte stream.
        printf("client --> %s",buffer);
        memset(buffer, 0, 1024*sizeof(char)); 
    }
    return status;
}