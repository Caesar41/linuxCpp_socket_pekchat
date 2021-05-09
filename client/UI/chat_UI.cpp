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

extern int user_id;
extern string user_name;

int chat_UI_frame(int tid, const string& msg, const string& func) {
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
    cout << "2021-09-12 11:12:00  eno" << endl;
    cout << "  -Hello, kumo" << endl;
    cout << "2021-09-12 11:13:05  kumo" << endl;
    cout << "  -Hello, eno" << endl;
    cout << "2021-09-12 11:13:20  kumo" << endl;
    cout << "  -I love u" << endl;
    cout << "2021-09-12 11:15:11  eno" << endl;
    cout << "  -me too" << endl;
    cout << "2021-09-12 11:15:33  kumo" << endl;
    cout << "  -OOOOOOHHHHH!!!" << endl;
    cout << "==========================" << endl;
    cout << "  \'/h\' to see history messages" << endl;
    cout << "  \'/e\' to exit this chat room" << endl;
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
        while (chat_buf.empty()) {
            getline(cin, chat_buf);
        }
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
        } else {
            send_chat(user_id, tid, chat_buf);
            msg = "send the message: " + string(chat_buf);
            continue;
        }
    }
    return 0;
}