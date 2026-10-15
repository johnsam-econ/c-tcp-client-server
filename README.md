# c-tcp-client-server

A simple TCP client/server application written in C using POSIX sockets. The server listens for incoming client connections on a configured port and prints received messages, while the client connects to the server and lets the user send text messages.

## Features

- TCP socket communication over IPv4
- Multi-step client menu for sending or exiting
- Graceful shutdown support with Ctrl+C on the server
- Easy build workflow with a Makefile

## Project structure

```text
.
├── config.h
├── makefile
├── client/
│   ├── client.c
│   ├── client.h
│   ├── main
│   └── main.c
├── server/
│   ├── main
│   ├── main.c
│   ├── server.c
│   └── server.h
└── README.md
```

## Requirements

- GCC or another C compiler
- Linux/macOS environment with POSIX sockets support
- Terminal access to run both programs

## Configuration

The shared network settings live in `config.h`:

- Port: `8080`
- Maximum queued connections: `3`
- Client actions: `SEND_DATA` and `EXIT`

You can modify these values before building if needed.

## Build

From the project root, run:

```bash
make
```

This builds both the server and client binaries:

- `server/main`
- `client/main`

You can also build them individually:

```bash
make server
make client
```

## Run

Start the server in one terminal:

```bash
./server/main
```

Then, in a second terminal, start the client:

```bash
./client/main
```

From the client menu, choose:

1. Send Message
2. Exit

The client sends the typed message to the server, and the server prints it to the terminal.

## Clean build artifacts

To remove generated binaries:

```bash
make clean
```

## Notes

- The client connects to `127.0.0.1` by default.
- The server listens on all local interfaces (`INADDR_ANY`) on the configured port.
- This project is intended as a lightweight learning example for socket programming in C.
