#include <stdio.h>
#include <sys/socket.h>
#include <string.h>

#include "../config.h"

//function definition
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