//
// Created by kumo on 5/9/21.
//

#include "chat_UI.h"
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include "../function/container_client.h"
#include "../package/message.h"
#include "../function/friend_function.h"
#include "../function/account_function.h"
#include "../function/chat_function.h"
#include <algorithm>

extern int user_id;
extern string user_name;

int chat_UI_frame(int tid, const string &msg, const string &func) {
    system("clear");
    friend_client target;

    get_friend_from_map_by_id(tid, &target);

    string tname = target.get_name();
    bool tonline = target.is_online();

    // get_chats_from_vector(chats_got, tid);

    cout << "==========================" << endl;
    cout << "Welcome to PEKCHAT room, " << user_name << "!" << endl;
    cout << "--------------------" << endl;
    cout << "chat with: " << tname << ", state: ";
    if (tonline) {
        cout << "online" << endl;
    } else {
        cout << "offline" << endl;
    }
    cout << "--------------------" << endl;
    vector<chat> chats_f;
    get_chats_from_map_by_id(chats_f, tid);

    sort(chats_f.begin(), chats_f.end(), chat_order_dec);

    int i = 0;
    for (auto c : chats_f) {
        if (i >= 10) {
            break;
        }
        string sender_name;
        if (c.get_sender() == user_id) {
            sender_name = user_name;
        } else if (c.get_sender() == tid) {
            sender_name = target.get_name();
        }
        cout << c.get_time() << "  " << sender_name << endl << "  -" << c.get_content() << endl;
    }
    cout << "==========================" << endl;
    cout << "  \'/h\' to see history messages" << endl;
    cout << "  \'/e\' to exit this chat room" << endl;
    cout << "  enter to refresh the message or send message" << endl;
    cout << "==========================" << endl;
    if (!func.empty()) {
        cout << "[--" << func << "--]" << endl;
    }
    if (!msg.empty()) {
        cout << msg << endl;
    }
    cout << "==========================" << endl;
    return 0;
}

int chat_UI_main(int tid) {
    string msg = "Please input the message to send or the command";
    string func = "chat room";
    while (true) {
        chat_UI_frame(tid, msg, func);
        int op;
        string chat_buf;
        // char chat_buf[1024];
        getline(cin, chat_buf);
        if (chat_buf.size() >= CONTENT_LENGTH) {
            msg = "message too long";
            continue;
        }
        if (chat_buf == "/h") {
            func = "history";
            msg = "now, this seem to has no effect";
        } else if (chat_buf == "/e") {
            cout << "exit this chat room!" << endl;
            sleep(1);
            break;
        } else if (chat_buf.empty()) {
            msg = "Please input the message to send or the command";
            func = "chat room";
            continue;
        } else {
            send_chat(user_id, tid, chat_buf);
            msg = "send the message: " + string(chat_buf);
            continue;
        }
    }
    return 0;
}