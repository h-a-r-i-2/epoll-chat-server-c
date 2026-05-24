#include "common.h"

pthread_t workers[WORKER_COUNT];

volatile int running = 1;

void handle_sigint(int sig) {
	printf("\nShutting down server gracefully...\n");

	running = 0;

	pthread_cond_broadcast(&task_queue.not_empty);
	pthread_cond_broadcast(&task_queue.not_full);
}

int main() {

    int server_fd = create_server_socket();

    signal(SIGINT, handle_sigint);

    printf("Chat server started on port %d\n", PORT);

    log_file = fopen("server.log", "a");

    if (!log_file) {
        perror("fopen");
        return 1;
    }

    pthread_mutex_init(&task_queue.mutex, NULL);
    pthread_cond_init(&task_queue.not_empty, NULL);
    pthread_cond_init(&task_queue.not_full, NULL);

    for (int i = 0; i < WORKER_COUNT; i++) {

        pthread_create(&workers[i],
                       NULL,
                       worker_thread,
                       NULL);
    }

    run_server(server_fd);

    for (int i = 0; i < WORKER_COUNT; i++) {
	    pthread_join(workers[i], NULL);
    }

    fclose(log_file);
    close(server_fd);

    pthread_mutex_destroy(&task_queue.mutex);
    pthread_cond_destroy(&task_queue.not_empty);
    pthread_cond_destroy(&task_queue.not_full);

    return 0;
}
