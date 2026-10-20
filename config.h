/*
 * Shared configuration for the TCP client/server example.
 * These constants are used by both the client and the server.
 */

// TCP port used by the application.
#define PORT 8080

// Maximum number of pending connections queued by the listening server socket.
#define MAX_QUEUE_LEN 3

// Maximum number of client sockets the server tracks at one time.
#define MAX_CLIENT 5

// Result returned by recv when the peer closes the connection.
#define DISCONNECT 0

// Client menu actions.
#define SEND_DATA 1
#define EXIT 2

// General operation results and server/client failure codes.
#define ERROR -1
#define SUCCESS 0
#define SOCKET_CREATE_ERR -1
#define SOCKET_BIND_ERR -2
#define SOCKET_LISTEN_ERR -3
#define SOCKET_ACCEPT_ERR -4
#define RD_ERR -5
#define SOCKET_CONNECT_ERR -6
#define WR_ERR -7