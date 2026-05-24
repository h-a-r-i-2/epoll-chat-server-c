# High-Performance Multi-Client Chat Server

A scalable Linux chat server implemented in C using epoll, non-blocking sockets, pthreads, and producer-consumer architecture.

## Features

- Multi-client support
- epoll-based event loop
- Non-blocking sockets
- Producer-consumer architecture
- Thread pool
- Real-time message broadcasting
- Logging support
- Client disconnect handling

## Technologies

- C
- Linux sockets
- epoll
- pthreads
- mutex
- condition variables
- TCP/IP

## Build

make

## Run

./chat_server

## Test

Open multiple terminals:

nc localhost 9090
