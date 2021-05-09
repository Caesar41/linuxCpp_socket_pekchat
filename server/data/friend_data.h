//
// Created by kumo on 5/9/21.
//

#ifndef PEKCHAT_FRIEND_DATA_H
#define PEKCHAT_FRIEND_DATA_H

#include "../service/friend_service.h"
#include <map>

int get_friends_from_database(map<int, friend_server> &m);

#endif //PEKCHAT_FRIEND_DATA_H
