//
// Created by kumo on 5/9/21.
//

#ifndef PEKCHAT_CONTAINERS_H
#define PEKCHAT_CONTAINERS_H

#include "../package/message.h"
#include "friend_service.h"
#include <string>
#include <map>

using namespace std;

int init_containers();

// message queue
int get_message(message *m);

int add_message(message m);

//friends
int add_friend_to_map(int uid, string name, bool o);

int get_friends_from_map(map<int, friend_server> &m);

int friend_log_change_to_map(int uid, bool o);

bool is_online_by_id(int uid);

#endif //PEKCHAT_CONTAINERS_H
