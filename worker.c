#include "common.h"

static void broadcast_message(int sender_fd,
                              char *buffer,
                              int bytes) {

    for (int i = 0; i < MAX_CLIENTS; i++) {

        if (clients[i] != 0 &&
            clients[i] != sender_fd) {

            send(clients[i],
                 buffer,
                 bytes,
                 0);
        }
    }
}

void* worker_thread(void *arg) {

    while (running) {

        task_t task = dequeue_task();

	if (!running)
		break;

	if (task.bytes >= BUFFER_SIZE)
		task.bytes = BUFFER_SIZE-1;

        task.buffer[task.bytes] = '\0';

        printf("Client %d: %s",
               task.client_fd,
               task.buffer);

        log_message(task.buffer);

        broadcast_message(task.client_fd,
                          task.buffer,
                          task.bytes);
    }

    return NULL;
}
