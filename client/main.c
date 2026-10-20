/*
 * Client entry point.
 * This program creates a socket, connects to the configured server,
 * and handles a basic interactive message loop until the user exits.
 */

#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#include "../config.h"
#include "client.h"

// Connect to the configured server and handle menu actions until the user exits.
int main(){
    // Keep the socket descriptor initialized so shared cleanup can check it.
    int exit_code = SUCCESS;
    int sockfd = -1;
    struct sockaddr_in addr = {0};
    socklen_t addrlen = sizeof(addr);
    int choice = -1;
    int loop = 1;
    char buffer[1024];
    char *server_ip = "127.0.0.1";

    // Create the TCP socket used for the server connection.
    if(create_socket(&sockfd) == -1){
        perror("Error in creating the socket");
        exit_code = SOCKET_CREATE_ERR;
        goto close_all;
    }

    // Build the server address using the configured port and selected IP.
    if(addr_defn(&addr,server_ip) == ERROR){
        exit_code = SOCKET_CONNECT_ERR;
        goto close_all;
    }

    // Establish the connection before accepting menu commands.
    if(connect_socket(sockfd,addr,addrlen) == ERROR){
        exit_code = SOCKET_CONNECT_ERR;
        goto close_all;
    }

    // Keep handling menu choices until EXIT is selected or sending fails.
    while(loop){
        print_menu();
        choice = get_option();
        switch(choice){
            case SEND_DATA:
                if(send_data(buffer,sockfd) != 0){
                    exit_code = WR_ERR;
                    goto close_all;
                }  
                break;
            case EXIT:
                printf("Exiting..\n");
                loop = 0;
                break;
            default:
                printf("Invalid Option try again.\n");
        }
    }

    // Release the socket on normal exit and on any earlier error.
    close_all:
    if (sockfd != -1)close(sockfd);
    return exit_code;
}
