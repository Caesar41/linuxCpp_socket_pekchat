//
// Created by kumo on 5/9/21.
//

#include "friend_service.h"
#include "container_server.h"
#include "../package/client_sock.h"


int friend_log_change(int uid, bool o) {

    friend_log_change_to_map(uid, o);

    // alert all user online
    user_log_change(uid, o);

    return 0;
}

// by fd and uid
int init_friend(int fd, int uid) {
    map<int, friend_server> m;
    get_friends_from_map(m);
    auto it = m.begin();
    while (it != m.end()) {
        friend_server f_now = it->second;
        message friend_r = message(FRIEND_RECV);

        // int o = get_fd_by_id(f_now.get_id()) > 0? 1: 0;
        int o = f_now.is_online()? 1: 0;

        friend_r.set_str(0, f_now.get_name().c_str());
        friend_r.set_int(STATE, o);
        friend_r.set_int(ID_TOKEN, f_now.get_id());

        send_by_fd(fd, friend_r);
        it++;
    }
    return 0;
}

friend_server::friend_server() {
    userid = -1;
    username = "";
    online = false;
}

friend_server::friend_server(int id, string n, bool o) {
    userid = id;
    username = n;
    online = o;
}

int friend_server::get_id() {
    return userid;
}

string friend_server::get_name() {
    return username;
}

bool friend_server::is_online() {
    return online;
}

void friend_server::login() {
    online = true;
}

void friend_server::logout() {
    online = false;
}