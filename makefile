# Build rules for the C TCP client/server example.
CC = gcc
CFLAGS = -Wall -Wextra -std=c11
CPPFLAGS = -I.

SERVER_BIN = server/main
CLIENT_BIN = client/main

all: server client

server: $(SERVER_BIN)

client: $(CLIENT_BIN)

$(SERVER_BIN): server/main.c server/server.c config.h server/server.h
	$(CC) $(CFLAGS) $(CPPFLAGS) server/main.c server/server.c -o $(SERVER_BIN)

$(CLIENT_BIN): client/main.c client/client.c config.h client/client.h
	$(CC) $(CFLAGS) $(CPPFLAGS) client/main.c client/client.c -o $(CLIENT_BIN)

run-server: server
	./$(SERVER_BIN)

run-client: client
	./$(CLIENT_BIN)

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)

.PHONY: all server client run-server run-client clean
