#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include "../config.h"
#include "server.h"

extern int server_status;

// Run the server lifecycle: create, bind, listen, accept, and receive.
int main(){
    int exit_code = SUCCESS;
    int sockfd = -1, new_sockfd =-1;
    struct sockaddr_in addr;
    socklen_t addrlen = (socklen_t) sizeof(addr);
    char buffer[1024] = {'\0'};

    // Keep descriptors initialized so cleanup can safely skip unopened sockets.
    if(create_socket(&sockfd) == -1){
        exit_code = SOCKET_CREATE_ERR;
        goto close_all;
    }

    // Configure the local IPv4 address and listening port.
    addr_defn(&addr);

    // Reserve the configured address and port for this server.
    if(bind_socket(sockfd,addr,addrlen) == ERROR){
        exit_code = SOCKET_BIND_ERR;
        goto close_all;
    }

    // Prepare the socket to queue incoming client connections.
    if(listen_socket(&sockfd) != SUCCESS){
        exit_code = SOCKET_LISTEN_ERR;
        goto close_all;
    }

    // Let Ctrl+C stop the server loop so it can close its sockets cleanly.
    signal(SIGINT,&close_sig);
    // Accept one client; this call blocks until a connection arrives.
    if(accept_socket(&sockfd,&new_sockfd,&addr,&addrlen) == ERROR){
        if(server_status != 0) exit_code = SOCKET_ACCEPT_ERR;
        goto close_all;
    }

    // Receive from the accepted client until it disconnects or recv fails.
    while(server_status){
        switch(server_receive(new_sockfd,buffer,sizeof(buffer))){
            case ERROR:
                exit_code = RD_ERR;
                goto close_all;
            
            case DISCONNECT:
                // Wait for another client after the current client disconnects.
                printf("Reconnecting...\n");
                    if(accept_socket(&sockfd,&new_sockfd,&addr,&addrlen) == ERROR){
                        if(server_status != 0) exit_code = SOCKET_ACCEPT_ERR;
                        goto close_all;
                    }
                // exit_code = SUCCESS;
                // goto close_all;
        }
    }

    // Close whichever descriptors were successfully created before returning.
    close_all:
    if(new_sockfd != -1)close(new_sockfd);
    if(sockfd != -1) close(sockfd);
    return exit_code;
}