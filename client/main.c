#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#include "../config.h"
#include "client.h"

//function declaration
void print_menu();
int get_option();
int send_data(char *buffer, int sockfd);

//main
int main(){
    //definition
    int exit_code = SUCCESS;
    int sockfd = -1;
    struct sockaddr_in addr = {0};
    socklen_t addrlen = sizeof(addr);
    int choice = -1;
    int loop = 1;
    char buffer[1024];

    char *server_ip = "127.0.0.1";

    // socket creation
    if(create_socket(&sockfd) == -1){
        perror("Error in creating the socket");
        exit_code = SOCKET_CREATE_ERR;
        goto close_all;
    }

    //address defintion
    if(addr_defn(&addr,server_ip) == ERROR){
        exit_code = SOCKET_CONNECT_ERR;
        goto close_all;
    }

    //socket connection
    if(connect_socket(sockfd,addr,addrlen) == ERROR){
        exit_code = SOCKET_CONNECT_ERR;
        goto close_all;
    }

    //client
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

    close_all:
    if (sockfd != -1)close(sockfd);
    return exit_code;
}
