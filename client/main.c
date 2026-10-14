#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#include "../confing.h"

//function declaration
void print_menu();
int get_option();
int send_data(char *buffer, int sockfd);

//main
int main(){
    //definition
    int sockfd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    int choice = -1;
    int loop = 1;
    char buffer[1024];

    char server_ip[] = "127.0.0.1";

    // socket creation
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1){
        perror("Error in creating the socket");
        return SOCKET_CREATE_ERR;
    }
    printf("Socket created successfully\n");

    //address defintion
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    if(inet_pton(AF_INET,server_ip,&addr.sin_addr) != 1){
        perror("Invalid Ip Address");
        close(sockfd);
        return SOCKET_CONNECT_ERR;
    }

    //socket connection
    if(connect(sockfd,(struct sockaddr*) &addr,addrlen) == -1){
        perror("Error in connecting the socket");
        close(sockfd);
        return SOCKET_CONNECT_ERR;
    }
    printf("Server connected successfully...\n");

    //client
    while(loop){

        print_menu();
        choice = get_option();

        switch(choice){
            case SEND_DATA:
            if(send_data(buffer,sockfd) != 0){
                close(sockfd);
                return WR_ERR;
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
    close(sockfd);
    return 0;
}
