#include "common.h"

task_queue_t task_queue;

void enqueue_task(task_t *task) {

    pthread_mutex_lock(&task_queue.mutex);

    while (task_queue.count == QUEUE_SIZE) {

        pthread_cond_wait(&task_queue.not_full,
                          &task_queue.mutex);
    }

    task_queue.tasks[task_queue.rear] = *task;

    task_queue.rear =
        (task_queue.rear + 1) % QUEUE_SIZE;

    task_queue.count++;

    pthread_cond_signal(&task_queue.not_empty);

    pthread_mutex_unlock(&task_queue.mutex);
}

task_t dequeue_task() {

    pthread_mutex_lock(&task_queue.mutex);

    while (task_queue.count == 0 && running) {

        pthread_cond_wait(&task_queue.not_empty,
                          &task_queue.mutex);
    }

    if(!running) {
	    pthread_mutex_unlock(&task_queue.mutex);
	    task_t empty = {0};

	    return empty;
    }

    task_t task =
        task_queue.tasks[task_queue.front];

    task_queue.front =
        (task_queue.front + 1) % QUEUE_SIZE;

    task_queue.count--;

    pthread_cond_signal(&task_queue.not_full);

    pthread_mutex_unlock(&task_queue.mutex);

    return task;
}
