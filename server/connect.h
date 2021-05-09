//
// Created by kumo on 5/7/21.
//

#ifndef PEKCHAT_CONNECT_H
#define PEKCHAT_CONNECT_H

#define MAX_EVENT_NUMBER 128

int init_listen_socket();
int run_epoll(int listen_fd, int *epoll_fd_s);

#endif //PEKCHAT_CONNECT_H