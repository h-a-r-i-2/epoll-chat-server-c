#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <signal.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/epoll.h>

#define PORT 9090
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 100
#define QUEUE_SIZE 256
#define WORKER_COUNT 4

extern volatile int running;

typedef struct {
    int client_fd;
    char buffer[BUFFER_SIZE];
    int bytes;
} task_t;

typedef struct {
    task_t tasks[QUEUE_SIZE];
    int front;
    int rear;
    int count;

    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;

} task_queue_t;

extern int clients[MAX_CLIENTS];
extern task_queue_t task_queue;
extern FILE *log_file;

int create_server_socket();
void run_server(int server_fd);

void enqueue_task(task_t *task);
task_t dequeue_task();

void* worker_thread(void *arg);

void log_message(const char *msg);

#endif
