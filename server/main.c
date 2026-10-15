#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../config.h"
#include "server.h"

int main(){
    int exit_code = SUCCESS;
    int sockfd = -1, new_sockfd =-1;
    int status = -1;
    struct sockaddr_in addr;
    socklen_t addrlen = (socklen_t) sizeof(addr);
    char buffer[1024] = {'\0'};

    //socket creation
    if(create_socket(&sockfd) == -1){
        exit_code = SOCKET_CREATE_ERR;
        goto close_all;
    }

    //address definition
    addr_defn(&addr);

    //socket binding
    if(bind_socket(sockfd,addr,addrlen) == ERROR){
        exit_code = SOCKET_BIND_ERR;
        goto close_all;
    }

    //socket listening
    if(listen_socket(&sockfd) != SUCCESS){
        exit_code = SOCKET_LISTEN_ERR;
        goto close_all;
    }

    //socket acception
    if(accept_socket(&sockfd,&new_sockfd,&addr,&addrlen) == ERROR){
        exit_code = SOCKET_ACCEPT_ERR;
        goto close_all;
    }

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