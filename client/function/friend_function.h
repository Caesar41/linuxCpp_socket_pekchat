//
// Created by kumo on 5/9/21.
//

#ifndef PEKCHAT_FRIEND_FUNCTION_H
#define PEKCHAT_FRIEND_FUNCTION_H

#include <string>
#include "../package/message.h"

using namespace std;

class friend_client {
    int userid;
    string username;
    bool online;

public:
    friend_client(int id, string n, bool o);
    friend_client();
    int get_id();
    string get_name();
    bool is_online();
    void login();
    void logout();
};

int add_friend(message m);

int friend_log_change(message m);

#endif //PEKCHAT_FRIEND_FUNCTION_H
