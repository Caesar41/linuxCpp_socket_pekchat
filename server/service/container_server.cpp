//
// Created by kumo on 5/9/21.
//

#include "container_server.h"
#include "../package/message.h"
#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include <cstdio>
#include "../data/friend_data.h"
#include "../data/chat_data.h"

using namespace std;

pthread_cond_t message_queue_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t message_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
queue<message *> message_to_handle;

int add_message(message m) {
    pthread_mutex_lock(&message_queue_mutex);
    {
        message_to_handle.push(new message(m));
    }
    pthread_mutex_unlock(&message_queue_mutex);
    pthread_cond_broadcast(&message_queue_cond);
    return 0;
}

// get message from queue and POP IT
int get_message(message *m) {
    pthread_mutex_lock(&message_queue_mutex);
    {
        while (message_to_handle.empty()) {
            pthread_cond_wait(&message_queue_cond, &message_queue_mutex);
        }
        *m = *message_to_handle.front();
        message_to_handle.pop();
    }
    pthread_mutex_unlock(&message_queue_mutex);
    return 0;
}


// this is used only for database, server should copy it when sending, though this will waste the memory
map<int, friend_server> users;
pthread_mutex_t friend_mutex;

int add_friend_to_map(int uid, string name, bool o) {
    pthread_mutex_lock(&friend_mutex);
    {
        users[uid] = friend_server(uid, name, o);
    }
    pthread_mutex_unlock(&friend_mutex);
    return 0;
}

int get_friends_from_map(map<int, friend_server> &m) {
    pthread_mutex_lock(&friend_mutex);
    {
        m = users;
    }
    pthread_mutex_unlock(&friend_mutex);
    return 0;
}

bool is_online_by_id(int uid) {
    bool r;
    pthread_mutex_lock(&friend_mutex);
    {
        r = users[uid].is_online();
    }
    pthread_mutex_unlock(&friend_mutex);
    return r;
}

int friend_log_change_to_map(int uid, bool o) {
    pthread_mutex_lock(&friend_mutex);
    {
        if (o) {
            users[uid].login();
        } else {
            users[uid].logout();
        }
    }
    pthread_mutex_unlock(&friend_mutex);
    return 0;
}

// chat, key is id, vector has all about him(sender or receiver)
map<int, vector<chat>> chats;
pthread_mutex_t chat_mutex;

int add_chat_to_map(chat c) {
    pthread_mutex_lock(&chat_mutex);
    if (chats[c.get_sender()].empty()) {
        vector<chat> v = {c};
        chats[c.get_sender()] = v;
    } else {
        chats[c.get_sender()].push_back(c);
    }
    if (chats[c.get_receiver()].empty()) {
        vector<chat> v = {c};
        chats[c.get_receiver()] = v;
    } else {
        chats[c.get_receiver()].push_back(c);
    }
    pthread_mutex_unlock(&chat_mutex);
    return 0;
}

int add_chat_to_map(message m) {
    chat c = chat(m);
    pthread_mutex_lock(&chat_mutex);
    add_chat_to_map(c);
    pthread_mutex_unlock(&chat_mutex);
    return 0;
}

int get_chats_from_map_by_id(vector<chat> &c, int uid) {
    pthread_mutex_lock(&chat_mutex);
    c = chats[uid];
    pthread_mutex_unlock(&chat_mutex);
    return 0;
}

int init_containers() {
    get_friends_from_database(users);
    get_chats_from_database();
    return 0;
}