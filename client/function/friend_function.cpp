//
// Created by kumo on 5/9/21.
//

#include "friend_function.h"
#include "container_client.h"


int add_friend(message m) {
    int uid = m.get_int(ID_TOKEN);
    string uname = m.get_str(0);
    bool o = m.get_int(STATE) == 1;

    add_friend_to_map(uid, uname, o);

    return 0;
}

int friend_log_change(message m) {
    int uid = m.get_int(ID_TOKEN);
    bool o = m.get_int(STATE) == 1;

    friend_log_change_to_map(uid, o);

    return 0;
}


friend_client::friend_client() {
    userid = -1;
    username = "";
    online = false;
}

friend_client::friend_client(int id, string n, bool o) {
    userid = id;
    username = n;
    online = o;
}

int friend_client::get_id() {
    return userid;
}

string friend_client::get_name() {
    return username;
}

bool friend_client::is_online() {
    return online;
}

void friend_client::login() {
    online = true;
}

void friend_client::logout() {
    online = false;
}