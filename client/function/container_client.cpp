//
// Created by kumo on 5/9/21.
//

#include "container_client.h"
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <cstdio>
#include <vector>

using namespace std;

extern int user_id;
extern string user_name;

vector<message *> response_message;
pthread_cond_t response_cond;
pthread_mutex_t response_mutex;

bool has_and_get_response(int t, message *m) {
    vector<message *>::iterator i;
    for (i = response_message.begin(); i != response_message.end(); i++) {
        int ty;
        (*i)->get_int(TYPE, &ty);
        if (ty == t) {
            *m = **i;
            response_message.erase(i);
            return true;
        }
    }
    return false;
}


int add_response(message m) {

    pthread_mutex_lock(&response_mutex);
    {
        response_message.push_back(new message(m));
    }
    pthread_mutex_unlock(&response_mutex);

    pthread_cond_broadcast(&response_cond);
    return 0;
}

// get and delete
int get_response(int t, message *m) {
    pthread_mutex_lock(&response_mutex);
    {
        while (!has_and_get_response(t, m)) {
            pthread_cond_wait(&response_cond, &response_mutex);
        }
    }
    pthread_mutex_unlock(&response_mutex);
    return 0;
}

// this is used only for server, UI should copy it, though this will waste the memory
// key is id
map<int, friend_client> friends;
pthread_mutex_t friend_mutex;

int add_friend_to_map(int uid, string name, bool o) {
    pthread_mutex_lock(&friend_mutex);
    {
        friends[uid] = friend_client(uid, name, o);
    }
    pthread_mutex_unlock(&friend_mutex);
    return 0;
}

int get_friends_from_map(map<int, friend_client> &m) {
    pthread_mutex_lock(&friend_mutex);
    {
        m = friends;
    }
    pthread_mutex_unlock(&friend_mutex);
    return 0;
}

int get_friend_from_map_by_id(int uid, friend_client *m) {
    pthread_mutex_lock(&friend_mutex);
    {
        *m = friends[uid];
    }
    pthread_mutex_unlock(&friend_mutex);
    return 0;
}

int friend_log_change_to_map(int uid, bool o) {
    pthread_mutex_lock(&friend_mutex);
    {
        if (o) {
            friends[uid].login();
        } else {
            friends[uid].logout();
        }
    }
    pthread_mutex_unlock(&friend_mutex);
    return 0;
}

int init_containers() {
    return 0;
}