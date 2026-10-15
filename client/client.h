void print_menu();
int get_option();
int send_data(char *buffer, int sockfd);
int create_socket(int *fd);
int connect_socket(int fd,struct sockaddr_in addr,socklen_t addrlen);