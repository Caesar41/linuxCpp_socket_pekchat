//
// Created by kumo on 5/7/21.
//

#include "connect.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "log/log.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <map>
#include "package/client_sock.h"
#include <semaphore.h>
#include <queue>
#include "service//handler_server.h"

using namespace std;

extern log log_file;
extern char log_buf[LOG_BUF_SIZE];



// return fd of listen socket
int init_listen_socket() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd < 0) {
        log_file.write_error("socket() failed");
        return -1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(5005);

    if (bind(listen_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        close(listen_fd);
        log_file.write_error("bind failed");
        return -1;
    }

    if (listen(listen_fd, 5) < 0) {
        close(listen_fd);
        log_file.write_error("listen failed");
        return -1;
    }

    sprintf(log_buf, "(listen_fd = %d) listensock init successfully", listen_fd);
    log_file.write_log(log_buf);

    return listen_fd;
}

int run_epoll(int listen_fd, int *epoll_fd_s) {
    // create fd for epoll
    int epoll_fd = epoll_create(1);
    *epoll_fd_s = epoll_fd;

    // create event for listen_fd
    struct epoll_event listen_event;
    listen_event.data.fd = listen_fd;
    listen_event.events = EPOLLIN;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &listen_event);

    log_file.write_log("epoll begin to work");

    // listen for events. listen socket will be handled now, client event will be done in another thread
    while (true) {
        struct epoll_event epoll_events[MAX_EVENT_NUMBER];

        int event_n = epoll_wait(epoll_fd, epoll_events, MAX_EVENT_NUMBER, -1);

        // error
        if (event_n < 0) {
            log_file.write_error("epoll_wait error");
            break;
        }

        // timeout (never)
        if (event_n == 0) {
            log_file.write_error("epoll_wait time out");
            break;
        }

        for (int i = 0; i < event_n; i++) {
            int e_fd = epoll_events[i].data.fd;
            uint32_t e_event = epoll_events[i].events;
            if (e_fd == listen_fd && (e_event & EPOLLIN)) {
                // new client
                struct sockaddr_in new_client;
                socklen_t len_t = sizeof(new_client);
                int client_fd = accept(listen_fd, (sockaddr *) &new_client, &len_t);
                if (client_fd < 0) {
                    log_file.write_error("accept failed");
                    // error
                } else {
                    struct epoll_event client_event;
                    client_event.events = EPOLLIN;
                    client_event.data.fd = client_fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event);

                    add_sock(client_fd);

                    sprintf(log_buf, "(client fd = %d) client connected, ip = %s, port = %d", client_fd,
                            inet_ntoa(new_client.sin_addr), new_client.sin_port);
                    log_file.write_log(log_buf);
                }
            } else if (e_event & EPOLLIN) {
                // client event
                ssize_t i_size = read_by_fd(e_fd);

                if (i_size <= 0) {
                    sprintf(log_buf, "(client fd = %d) client disconnected, return code is %ld", e_fd, i_size);
                    log_file.write_log(log_buf);

                    // delete from epoll
                    epoll_event del;
                    del.events = EPOLLIN;
                    del.data.fd = e_fd;

                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, e_fd, &del);
                    close(e_fd);

                    // delete from map
                    delete_sock(e_fd);
                    continue;
                }

            }
        }

    }
    log_file.write_log("epoll exit, chat process end");
    close(epoll_fd);
    return 0;
}