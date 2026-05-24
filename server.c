#include "common.h"

int clients[MAX_CLIENTS] = {0};

static int set_nonblocking(int fd) {

    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
        return -1;

    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int create_server_socket() {

    int server_fd;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;

    setsockopt(server_fd,
               SOL_SOCKET,
               SO_REUSEADDR,
               &opt,
               sizeof(opt));

    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd,
             (struct sockaddr*)&server_addr,
             sizeof(server_addr)) == -1) {

        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, SOMAXCONN) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    set_nonblocking(server_fd);

    return server_fd;
}

void run_server(int server_fd) {

    int epoll_fd;

    struct epoll_event ev;
    struct epoll_event events[MAX_EVENTS];

    epoll_fd = epoll_create1(0);

    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    epoll_ctl(epoll_fd,
              EPOLL_CTL_ADD,
              server_fd,
              &ev);

    while (running) {

        int nfds = epoll_wait(epoll_fd,
                              events,
                              MAX_EVENTS,
                              -1);

	if (nfds == -1) {
		if (errno == EINTR)
			continue;

		perror("epoll_wait");
		break;
	}

        for (int i = 0; i < nfds; i++) {

            if (events[i].data.fd == server_fd) {

                struct sockaddr_in client_addr;
                socklen_t client_len =
                    sizeof(client_addr);

                int client_fd = accept(server_fd,
                                       (struct sockaddr*)&client_addr,
                                       &client_len);

                if (client_fd == -1)
                    continue;

                set_nonblocking(client_fd);

                printf("Client connected FD=%d\n",
                       client_fd);

                ev.events = EPOLLIN;
                ev.data.fd = client_fd;

                epoll_ctl(epoll_fd,
                          EPOLL_CTL_ADD,
                          client_fd,
                          &ev);

                for (int j = 0; j < MAX_CLIENTS; j++) {

                    if (clients[j] == 0) {
                        clients[j] = client_fd;
                        break;
                    }
                }

            } else {

                int client_fd = events[i].data.fd;

                task_t task;

                task.client_fd = client_fd;

                task.bytes = recv(client_fd,
                                  task.buffer,
                                  BUFFER_SIZE,
                                  0);

                if (task.bytes <= 0) {

                    printf("Client disconnected FD=%d\n",
                           client_fd);

                    epoll_ctl(epoll_fd,
                              EPOLL_CTL_DEL,
                              client_fd,
                              NULL);

                    close(client_fd);

                    for (int j = 0; j < MAX_CLIENTS; j++) {

                        if (clients[j] == client_fd) {
                            clients[j] = 0;
                            break;
                        }
                    }

                } else {

                    enqueue_task(&task);
                }
            }
        }
    }

    for (int i = 0; i < MAX_CLIENTS; i++) {
	    if (clients[i] != 0) {
		    close(clients[i]);
	    }
    }

    close(epoll_fd);
}
