//
// Created by kumo on 5/8/21.
//

#ifndef PEKCHAT_CLIENT_SOCK_H
#define PEKCHAT_CLIENT_SOCK_H

#include "message.h"
#include <string>

using namespace std;

class client_sock {
    int fd;
    long int buf_size;
    char buf[MESSAGE_LENGTH];
    char send_buf[MESSAGE_LENGTH];
    int user_id;

public:
    client_sock();

    client_sock(int fd);

    int client_read();

    int get_fd();

    int get_id();

    void set_id(int uid);

    long int client_send(message m);
};

//if offline, return -1
int get_fd_by_id(int uid);

void user_login_sock(int fd, int uid);

void add_sock(int fd);

void delete_sock(int fd);

long int read_by_fd(int fd);

int send_by_fd(int fd, message m);

void close_client_sock();

void user_log_change(int uid, bool o);

void new_user_sock(int uid, string uname);

void send_server_time(int fd_s);

#endif //PEKCHAT_CLIENT_SOCK_H