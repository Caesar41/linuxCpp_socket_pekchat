//
// Created by kumo on 5/8/21.
//

#include "client_sock.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include "../log/log.h"
#include "../service/container_server.h"
#include <map>
#include <unistd.h>

using namespace std;

extern log log_file;
extern char log_buf[LOG_BUF_SIZE];
map<int, client_sock *> client_socks;  // fd is key

void send_server_time(int fd_s) {
    message time_res = message(TIME_RESPONSE);
    time_t server_t;
    time(&server_t);
    time_res.set_int(TIME_SERVER, (int) server_t);
    send_by_fd(fd_s, time_res);
}

//if offline, return -1
int get_fd_by_id(int uid) {
    map<int, client_sock *>::iterator it;
    for (it = client_socks.begin(); it != client_socks.end(); it++) {
        if (it->second->get_id() == uid) {
            return it->second->get_fd();
        }
    }
    return -1;
}

void user_login_sock(int fd, int uid) {
    client_socks[fd]->set_id(uid);
}


void add_sock(int fd) {
    client_socks[fd] = new client_sock(fd);
}

void delete_sock(int fd) {
    client_socks.erase(fd);
}

long int read_by_fd(int fd) {
    return client_socks[fd]->client_read();
}

int send_by_fd(int fd, message m) {
    return (int) client_socks[fd]->client_send(m);
}

void close_client_sock() {
    map<int, client_sock *>::iterator it;
    for (it = client_socks.begin(); it != client_socks.end(); it++) {
        close(it->second->get_fd());
    }
}

void user_log_change(int uid, bool o) {
    map<int, client_sock *>::iterator it;
    for (it = client_socks.begin(); it != client_socks.end(); it++) {
        message log_change = message(FRIEND_LOG_CHANGE);
        log_change.set_int(ID_TOKEN, uid);
        if (o) {
            log_change.set_int(STATE, 1);
        } else {
            log_change.set_int(STATE, 0);
        }
        it->second->client_send(log_change);
    }
}

void new_user_sock(int uid, string uname) {
    map<int, client_sock *>::iterator it;
    for (it = client_socks.begin(); it != client_socks.end(); it++) {
        message user_recv = message(FRIEND_RECV);
        user_recv.set_int(ID_TOKEN, uid);
        user_recv.set_str(0, uname.c_str());
        user_recv.set_int(STATE, 0);
        it->second->client_send(user_recv);
    }
}

long int client_sock::client_send(message m) {
    m.serialize(send_buf);

    long int r = send(fd, send_buf, MESSAGE_LENGTH, 0);

    return r;
}

client_sock::client_sock() {
    this->fd = -1;
    this->buf_size = MESSAGE_LENGTH;
    memset(this->buf, 0, MESSAGE_LENGTH);
}

client_sock::client_sock(int fd) {
    this->fd = fd;
    this->buf_size = MESSAGE_LENGTH;
    memset(this->buf, 0, MESSAGE_LENGTH);
}

int client_sock::get_id() {
    return user_id;
}

int client_sock::get_fd() {
    return fd;
}

void client_sock::set_id(int uid) {
    user_id = uid;
}

int client_sock::client_read() {
    ssize_t in_size = recv(fd, &buf[MESSAGE_LENGTH - buf_size], buf_size, 0);
    if (in_size <= 0) {
        // client shut down
    } else {
        buf_size -= in_size;
        if (buf_size == 0) {
            buf_size = MESSAGE_LENGTH;
            // handle the message
            sprintf(log_buf, "(client fd = %d) receive message", fd);
            log_file.write_log(log_buf);
            message *m = new message;
            m->parse(buf);
            m->set_fd(fd);
            add_message(*m);
        }
    }
    return (int) in_size;
}