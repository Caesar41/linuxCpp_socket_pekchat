//
// Created by kumo on 5/8/21.
//

#ifndef PEKCHAT_ACCOUNT_SERVICE_H
#define PEKCHAT_ACCOUNT_SERVICE_H

#include "../package/message.h"
#include <string>

using namespace std;

int register_func(message m);

int login_func(message m);

void login_ok(message m);

void user_logout(message m);

void add_new_user(int uid, string username, bool o);

#endif //PEKCHAT_ACCOUNT_SERVICE_H
