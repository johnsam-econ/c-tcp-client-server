#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../config.h"

//function definition
int create_socket(int *fd){
    *fd = socket(AF_INET,SOCK_STREAM,0);
    if(*fd == ERROR){
        perror("Error in binding the socket");
        return ERROR;
    }
    printf("Socket created successfully\n");
    return SUCCESS;
}

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

int addr_defn(struct sockaddr_in *addr,char *ip){
    (*addr).sin_family = AF_INET;
    (*addr).sin_port = htons(PORT);
    if(inet_pton(AF_INET,ip,&addr->sin_addr) != 1){
        perror("Invalid Ip Address");
        return ERROR;
    }
    return SUCCESS;
}

void print_menu(){
    printf("=======MENU======\n");
    printf("1.Send Message.\n");
    printf("2.Exit.\n");
    printf("=================\n");
}

int get_option(){
    int x;
    printf("Enter the choice: ");
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

int send_data(char *buffer, int sockfd){
    printf("Enter the message:");

    if(fgets(buffer,1024,stdin) == NULL){
        return WR_ERR;
    }
    if(send(sockfd,buffer,strlen(buffer),0) == -1){
        perror("Error in sending data to the server");
        return WR_ERR;
    }
    printf("Data sent to server successfully\n"); 
    return 0;
}