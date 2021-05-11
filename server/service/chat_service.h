//
// Created by kumo on 5/11/21.
//

#ifndef PEKCHAT_CHAT_SERVICE_H
#define PEKCHAT_CHAT_SERVICE_H

#include <string>
#include "../package/message.h"

using namespace std;

class chat {
    int mid;
    int sender;
    int receiver;
    string content;
    int time_s;

public:
    chat();
    chat(message m);
    chat(int s, int r, string c, int t);
    chat(int cid, int s, int r, string c, int t);

    void to_message(message *m);
    int get_mid();
    int get_sender();
    int get_receiver();
    string get_content();
    string get_time();
    int get_time_int();
    bool operator< (const chat &c) const;
    bool operator> (const chat &c) const;
};

int got_chat_function(message m);

int init_chat(int fd, int uid);

#endif //PEKCHAT_CHAT_SERVICE_H
