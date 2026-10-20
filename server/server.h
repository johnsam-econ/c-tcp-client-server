/*
 * Public server helper declarations.
 * These functions are implemented in server.c and used by the server entry point.
 */

int listen_socket(int *fd);
int bind_socket(int fd, struct sockaddr_in addr, socklen_t addrlen);
int accept_socket(int *fd, int *new_fd, struct sockaddr_in *addr, socklen_t *addrlen);
void addr_defn(struct sockaddr_in *addr);
int server_receive(int fd, char *buffer, int len);
int create_socket(int *fd);

// SIGINT handler used to request graceful shutdown from the server loop.
void close_sig();