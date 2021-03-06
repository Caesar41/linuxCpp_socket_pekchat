//
// Created by kumo on 5/9/21.
//

#include "chat_function.h"
#include "container_client.h"
#include <time.h>
#include "../client.h"

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

void chat::to_message(message *m) {
    m->set_int(TYPE, CHAT_SEND);
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
    return c1 < c2;
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
    time_t tt = (long int) time_s;
    info = localtime(&tt);
    char buffer[100] = {0};
    strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", info);
    return string(buffer);
}

int send_chat(int source, int des, string content) {
    int server_t = get_server_time();
    chat new_chat = chat(source, des, content, server_t);

    message m = message(CHAT_SEND);
    new_chat.to_message(&m);

    // add chat to map
    add_chat(m);

    client_send(&m);
    return 0;
}

int add_chat(message m) {
    chat c = chat(m);
    add_chat_to_map(c);
    return 0;
}