//
// Created by kumo on 5/8/21.
//

#include "account_function.h"
#include "../package/message.h"
#include <sys/socket.h>
#include "../client.h"

extern char send_buf[MESSAGE_LENGTH];
extern int sock_fd;

int register_request(const char *username, const char *password) {
    message *m = new message(REGISTER_REQUEST);
    m->set_str(0, username);
    m->set_str(1, password);
    if (client_send(m) < 0) {
        return -1;
    }
    return 0;
}

int login_request(const char *username, const char *password) {
    message *m = new message(LOGIN_REQUEST);
    m->set_str(0, username);
    m->set_str(1, password);
    if (client_send(m) < 0) {
        return -1;
    }
    return 0;
}

int login_OK() {
    message *m = new message(LOGIN_OK);
    if (client_send(m) < 0) {
        return -1;
    }
    return 0;
}

int logout() {
    message *m = new message(LOGOUT);
    if (client_send(m) < 0) {
        return -1;
    }
    return 0;
}