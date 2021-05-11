//
// Created by kumo on 5/11/21.
//

#include "chat_service.h"
#include "container_server.h"
#include <time.h>
#include "../data/chat_data.h"
#include "../package/client_sock.h"

/*
    int mid;
    int sender;
    int receiver;
    string content;
    int time_s;
 */

chat::chat() {}

chat::chat(message m) {
    mid = m.get_int(ID_TOKEN);
    sender = m.get_int(SOURCE);
    receiver = m.get_int(DESTINATION);
    content = string(m.content);
    time_s = m.get_int(TIME_SERVER);
}

chat::chat(int s, int r, string c, int t) {
    mid = -1;
    sender = s;
    receiver = r;
    content = c;
    time_s = t;
}

chat::chat(int cid, int s, int r, string c, int t) {
    mid = cid;
    sender = s;
    receiver = r;
    content = c;
    time_s = t;
}

void chat::to_message(message *m) {
    m->set_int(TYPE, CHAT_RECV);
    m->set_int(ID_TOKEN, mid);
    m->set_int(SOURCE, sender);
    m->set_int(DESTINATION, receiver);
    m->set_int(TIME_SERVER, time_s);
    m->set_content(content.c_str());
}

bool chat::operator< (const chat &c) const {
    return time_s < c.time_s;
}

bool chat::operator> (const chat &c) const {
    return time_s > c.time_s;
}

bool chat_order_dec (const chat &c1, const chat &c2) {
    return c1 > c2;
}

int chat::get_mid() {
    return mid;
}

int chat::get_sender() {
    return sender;
}

int chat::get_receiver() {
    return receiver;
}

string chat::get_content() {
    return content;
}

string chat::get_time() {
    struct tm *info;
    info = localtime((long int *) &time_s);
    char buffer[100] = {0};
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
    return string(buffer);
}

int chat::get_time_int() {
    return time_s;
}

int got_chat_function(message m) {
    // add to data
    chat c = chat(m);
    add_chat_to_data(c);
    // add to cache (later, get chat id from database)
    add_chat_to_map(c);
    // if target online, send to him
    int recv_id = c.get_receiver();
    if (is_online_by_id(recv_id)) {
        int recv_fd = get_fd_by_id(recv_id);
        message chat_send = m;
        chat_send.set_int(TYPE, CHAT_RECV);
        send_by_fd(recv_fd, chat_send);
    }
    return 0;
}

// by fd and uid
int init_chat(int fd, int uid) {
    vector<chat> chats;
    get_chats_from_map_by_id(chats, uid);
    auto it = chats.begin();
    while (it != chats.end()) {
        message chat_r = message(CHAT_RECV);
        it->to_message(&chat_r);
        send_by_fd(fd, chat_r);
        it++;
    }
    return 0;
}