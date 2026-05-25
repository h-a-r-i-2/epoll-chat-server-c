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

## Sample Output

1. Server Side:
Chat server started on port 9090
Client connected FD=6
Client connected FD=7
Client 7: Hi
Client 6: Hi, How are you?
Client 7: Yes, I'm fine... How are you?
^C
Shutting down server gracefully...

2. Client FD=6:
Hi
Hi, How are you?
Yes, I'm fine... How are you?

3. Client FD=7:
Hi
Hi, How are you?
Yes, I'm fine... How are you?
