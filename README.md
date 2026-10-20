# c-tcp-client-server

A small TCP client/server example written in C using POSIX sockets. The server listens for incoming connections, accepts messages from clients, and prints them to the terminal. The client connects to the server and provides a simple menu for sending text or exiting cleanly.

## Overview

This project demonstrates a minimal socket-based client/server workflow in a Linux-style environment. It focuses on the core networking primitives used in many C applications:

- socket creation
- address configuration
- connection setup
- message sending and receipt
- graceful shutdown

## Features

- IPv4 TCP communication
- Simple interactive client menu
- Server-side loop that accepts multiple client connections up to the configured limit
- Ctrl+C shutdown handling for the server
- Easy build workflow using a Makefile

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
- Linux/macOS environment with POSIX socket support
- Terminal access to run the client and server concurrently

## Configuration

The shared network settings live in `config.h`:

- Port: `8080`
- Maximum queued connections: `3`
- Client menu actions: `SEND_DATA` and `EXIT`

These defaults can be adjusted before building if needed.

## Build

From the project root, run:

```bash
make
```

This builds both binaries:

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

Then start the client in a second terminal:

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
- The server binds to `INADDR_ANY`, which listens on all local interfaces for the configured port.
- This project is intended as a lightweight learning example for socket programming in C.
