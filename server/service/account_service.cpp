//
// Created by kumo on 5/8/21.
//

#include <cstdio>
#include "account_service.h"
#include "../package/client_sock.h"
#include "../data/account_data.h"
#include "../log/log.h"
#include "friend_service.h"
#include "container_server.h"

extern log log_file;
extern char log_buf[];

void add_new_user(int uid, string username, bool o) {

    new_user_sock(uid, username);

    add_friend_to_map(uid, username, o);
}

int register_func(message m) {
    char username[25], password[25];

    m.get_str(0, username);
    m.get_str(1, password);

    int r = register_data(username, password);

    sprintf(log_buf, "user register, username %s, password %s---", username, password);
    log_file.write_log(log_buf);
    if (r > 0) {
        add_new_user(r, username, false);
        log_file.write_log_f("success");
    } else if (r == -2) {
        log_file.write_log_f("failed, username exists");
    } else {
        log_file.write_log_f("failed, other error");
    }

    message response = message(REGISTER_RESPONSE);
    response.set_int(STATE, r);
    return send_by_fd(m.get_fd(), response);
}

void user_logout(message m) {
    int uid = m.get_int(SOURCE);
    friend_log_change(uid, false);
}

int login_func(message m) {
    char username[25], password[25];

    m.get_str(0, username);
    m.get_str(1, password);

    int r = login_data(username, password);

    sprintf(log_buf, "user login, username %s, password %s---", username, password);
    log_file.write_log(log_buf);
    if (r > 0) {
        // id is r
        if (is_online_by_id(r)) {
            sprintf(log_buf, "failed, already online");
            log_file.write_log_f(log_buf);
            r = -3;
        } else {
            sprintf(log_buf, "success, id = %d", r);
            log_file.write_log_f(log_buf);
        }
    } else if (r == -1) {
        log_file.write_log_f("failed, username not exists");
    } else if (r == -2) {
        log_file.write_log_f("failed, wrong password");
    } else {
        log_file.write_log_f("failed, other error");
    }

    message response = message(LOGIN_RESPONSE);
    if (r > 0) {
        response.set_int(STATE, 0);
        response.set_int(ID_TOKEN, r);
    } else {
        response.set_int(STATE, r);
    }
    return send_by_fd(m.get_fd(), response);
}

void login_ok(message m) {
    int id = m.get_int(SOURCE);
    int fd = m.get_fd();

    // add it's online state, init this user, notice other users

    user_login_sock(fd, id);

    init_friend(fd, id);

    // init message

    message response = message(INIT_COMPLETE);

    send_by_fd(fd, response);

    friend_log_change(id, true);

    sprintf(log_buf, "user login completed, id = %d", id);
    log_file.write_log(log_buf);

}