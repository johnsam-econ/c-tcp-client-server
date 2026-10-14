#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../confing.h"
#include "server.h"

int main(){
    int exit_code = SUCCESS;
    int sockfd = -1, new_sockfd =-1;
    int status = -1;
    struct sockaddr_in addr;
    socklen_t addrlen = (socklen_t) sizeof(addr);
    char buffer[1024] = {'\0'};

    //socket creation
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        perror("Error in creating the socket");
        exit_code = SOCKET_CREATE_ERR;
        goto close_all;
    }
    printf("Socket created successfully\n");

    //address definition
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    //socket binding
    if(bind(sockfd,(struct sockaddr*) &addr,addrlen) == -1){
        perror("Error in binding the socket");
        exit_code = SOCKET_BIND_ERR;
        goto close_all;
    }
    printf("Binding created successfully\n");

    //socket listening
    if(listen(sockfd,MAX_QUEUE_LEN) == -1){
        perror("Error in listening the Socket");
        exit_code = SOCKET_LISTEN_ERR;
        goto close_all;
    }
    printf("listen done successfully\n");

    //socket acception
    new_sockfd = accept(sockfd,(struct sockaddr *) &addr,&addrlen);
    if(new_sockfd == -1){
        perror("Error in accepting the new socket");
        exit_code = SOCKET_ACCEPT_ERR;
        goto close_all;
    }
    printf("Client Connected sucessfully...\n");

    //server
    while(1){
        status = recv(new_sockfd,buffer,sizeof(buffer),0);
        if(status == -1){
            perror("Error in Reading the data from the server");
            exit_code = RD_ERR;
            goto close_all;
        }
        if(status == 0){
            printf("Client Disconnected..\n");
            break;
        }
        printf("client --> %s",buffer);
    }


    close_all:
    if(new_sockfd != -1)close(new_sockfd);
    if(sockfd != -1) close(sockfd);
    return exit_code;
}