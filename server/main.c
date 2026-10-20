#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <errno.h>

#include "../config.h"
#include "server.h"

extern int server_status;

// Run the server lifecycle: create, bind, listen, accept, and receive.
int main(){
    // int pid = -1;
    int exit_code = SUCCESS;
    int sockfd = -1;
    int new_sockfd[MAX_CLIENT];
    for (int i = 0; i < MAX_CLIENT; i++) new_sockfd[i] = -1;
    int count_sockfd = 0;
    struct sockaddr_in addr;
    socklen_t addrlen = (socklen_t) sizeof(addr);
    char buffer[1024] = {'\0'};

    // readfds is the working set passed to select; masterfds tracks all sockets.
    fd_set readfds;
    fd_set masterfds;
    // select only checks descriptors up to maxfd.
    int maxfd = 0;

    FD_ZERO(&readfds);

    // Keep descriptors initialized so cleanup can safely skip unopened sockets.
    if(create_socket(&sockfd) == -1){
        exit_code = SOCKET_CREATE_ERR;
        goto close_all;
    }
    FD_SET(sockfd, &readfds);
    if (sockfd > maxfd) maxfd = sockfd;

    // Configure the local IPv4 address and listening port.
    addr_defn(&addr);

    // Reserve the configured address and port for this server.
    if (bind_socket(sockfd, addr, addrlen) == ERROR) {
        exit_code = SOCKET_BIND_ERR;
        goto close_all;
    }

    // Prepare the socket to queue incoming client connections.
    if (listen_socket(&sockfd) != SUCCESS) {
        exit_code = SOCKET_LISTEN_ERR;
        goto close_all;
    }

    // Let Ctrl+C stop the server loop so it can close its sockets cleanly.
    signal(SIGINT,&close_sig);

    // Monitor the listening socket and connected clients for readable data.
    masterfds = readfds;
    while (server_status) {
        // select modifies readfds, so restore it from the tracked socket set.
        readfds = masterfds;
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) < 0) {
            if(errno == EINTR && server_status == 0){
                break;
            }

            perror("Select");
            exit_code = ERROR;
            goto close_all;
        }

        for (int i = 0; i < maxfd + 1; i++) {
            if (FD_ISSET(sockfd, &readfds) && sockfd == i) {
                // Accept clients only while a slot is available.
                if (count_sockfd < MAX_CLIENT) {
                    for (int j = 0; j < MAX_CLIENT; j++) {
                        if (new_sockfd[j] == -1) {
                            if (accept_socket(&sockfd, &new_sockfd[j], &addr, &addrlen) == ERROR) {
                                if (server_status != 0) exit_code = SOCKET_ACCEPT_ERR;
                                goto close_all;
                            } else {
                                FD_SET(new_sockfd[j], &masterfds);
                                if (new_sockfd[j] > maxfd) maxfd = new_sockfd[j];
                                count_sockfd++;
                                printf("Client %d Connected sucessfully\n",j);
                                break;
                            }
                        }
                    }
                } else {
                    // Stop monitoring the listener until a client slot opens.
                    FD_CLR(sockfd, &masterfds);
                    printf("Max Client connected\n");
                }
            }
                if (FD_ISSET(i, &readfds) && i != sockfd) {
                    switch (server_receive(i, buffer, sizeof(buffer))) {
                        case ERROR:
                            exit_code = RD_ERR;
                            goto close_all;

                        case DISCONNECT:
                            // Remove the disconnected client and release its slot.
                            close(i);
                            count_sockfd--;
                            FD_CLR(i, &masterfds);
                            // Recalculate the scan limit if the highest fd closed.
                            if (maxfd == i) {
                                for (int k = maxfd - 1; k >= 0; k--) {
                                    if (FD_ISSET(k, &masterfds)) {
                                        maxfd = k;
                                        break;
                                    }
                                }
                            }
                            // Resume accepting connections once capacity is available.
                            if (!FD_ISSET(sockfd, &masterfds) && count_sockfd < MAX_CLIENT) {
                                FD_SET(sockfd, &masterfds);
                                printf("Server started to listening..\n");
                            }
                            for (int k = 0; k < MAX_CLIENT; k++) {
                                if (new_sockfd[k] == i) {
                                    new_sockfd[k] = -1;
                                    break;
                                }
                            }
                    }
                }
            
        }
    }

    // Close all client sockets and the listening socket before returning.
close_all:
    printf("\nServer Terminating\n");

    for (int i = 0; i < MAX_CLIENT; i++) {
        if (new_sockfd[i] != -1) close(new_sockfd[i]);
    }
    // if(new_sockfd != -1)close(new_sockfd);
    if (sockfd != -1) close(sockfd);
    return exit_code;
}