//
// Created by kumo on 5/9/21.
//

#ifndef PEKCHAT_CONTAINERS_H
#define PEKCHAT_CONTAINERS_H

#include "../package/message.h"
#include "friend_function.h"
#include <map>

using namespace std;

int init_containers();

// response
int get_response(int t, message *m);

int add_response(message m);

bool has_and_get_response(int t, message *m);

//friends
int add_friend_to_map(int uid, string name, bool o);

int get_friends_from_map(map<int, friend_client> &m);

int friend_log_change_to_map(int uid, bool o);

int get_friend_from_map_by_id(int uid, friend_client *m);

#endif //PEKCHAT_CONTAINERS_H
