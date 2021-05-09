//
// Created by kumo on 5/9/21.
//

#ifndef PEKCHAT_CHAT_FUNCTION_H
#define PEKCHAT_CHAT_FUNCTION_H

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

    void to_message(message *m);
    int get_mid();
    int get_sender();
    int get_receiver();
    string get_content();
    string get_time();
};

int send_chat(int source, int des, string content);

#endif //PEKCHAT_CHAT_FUNCTION_H
