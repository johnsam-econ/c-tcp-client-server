/*
 * Client socket helpers.
 * This file keeps the network operations separate from higher-level menu logic.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../config.h"

// Create an IPv4 TCP socket and return its descriptor through fd.
int create_socket(int *fd){
    *fd = socket(AF_INET,SOCK_STREAM,0);
    if(*fd == ERROR){
        perror("Error in creating the socket");
        return ERROR;
    }
    printf("Socket created successfully\n");
    return SUCCESS;
}

// Connect the socket to the supplied server endpoint.
int connect_socket(int fd,struct sockaddr_in addr,socklen_t addrlen){
    if(connect(fd,(struct sockaddr *) &addr,addrlen) == -1){
        perror("Error in connecting the socket");
        return ERROR;
    }
    else{
        printf("Server connected successfully...\n");
        return SUCCESS;
    }
}

// Fill in the IPv4 server endpoint from the configured port and IP string.
int addr_defn(struct sockaddr_in *addr,char *ip){
    (*addr).sin_family = AF_INET;
    (*addr).sin_port = htons(PORT);
    if(inet_pton(AF_INET,ip,&addr->sin_addr) != 1){
        perror("Invalid Ip Address");
        return ERROR;
    }
    return SUCCESS;
}

// Display the actions the client accepts.
void print_menu(){
    printf("=======MENU======\n");
    printf("1.Send Message.\n");
    printf("2.Exit.\n");
    printf("=================\n");
    printf("Enter the choice: ");
}

// Read a menu selection and discard the rest of its input line.
int get_option(){
    int x;
    // printf("Enter the choice: ");
    
    scanf("%d",&x);
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF);

    if(x == 1 || x == 2){
        return x;
    }
    else{
        return -1;
    }
}

// Read one line from standard input and send its bytes to the server.
int send_data(char *buffer, int sockfd){
    // The client waits for a server status response after each message.
    int ack = ERROR;
    printf("Enter the message:");

    if(fgets(buffer,1024,stdin) == NULL){
        return WR_ERR;
    }
    if(send(sockfd,buffer,strlen(buffer),0) == ERROR){
        perror("Error in sending data to the server");
        return WR_ERR;
    }
    // Treat a missing response or a non-success status as a failed send.
    if(recv(sockfd,&ack,sizeof(int),0) == ERROR){
        perror("error in reciving the acknowledgement");
        return WR_ERR;
    }
    if(ack == SUCCESS){
        printf("Data sent to server successfully\n");
        ack =ERROR;
    }
    else{
        printf("Server Disconnected. Closing client...\n");
        return WR_ERR;
    }
    return SUCCESS;
}