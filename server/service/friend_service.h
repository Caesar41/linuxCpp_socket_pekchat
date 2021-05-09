//
// Created by kumo on 5/9/21.
//

// before we have "friend" instead of user, "friend" is same as user

#ifndef PEKCHAT_FRIEND_SERVICE_H
#define PEKCHAT_FRIEND_SERVICE_H

#include "string"
#include "../package/message.h"

using namespace std;

class friend_server {
    int userid;
    string username;
    bool online;

public:
    friend_server(int id, string n, bool o);
    friend_server();
    int get_id();
    string get_name();
    bool is_online();
    void login();
    void logout();
};

int init_friend(int fd, int uid);

int friend_log_change(int uid, bool o);
#endif //PEKCHAT_FRIEND_SERVICE_H
