int create_socket(int *fd);
int listen_socket(int *fd);
int bind_socket(int fd,struct sockaddr_in addr,socklen_t addrlen);