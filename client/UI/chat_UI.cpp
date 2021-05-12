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

    // first is the last
    sort(chats_f.begin(), chats_f.end(), chat_order_dec);

    auto c = max(chats_f.end() - 10, chats_f.begin());
    while (c != chats_f.end()) {
        string sender_name;
        if (c->get_sender() == user_id) {
            sender_name = user_name;
        } else if (c->get_sender() == tid) {
            sender_name = target.get_name();
        }
        cout << c->get_time() << "  " << sender_name << endl << "  -" << c->get_content() << endl;
        c++;
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
            history_UI(tid, 1);
            msg = "Please input the message to send or the command";
            func = "chat room";
            continue;
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
            msg = "message sent: " + string(chat_buf);
            continue;
        }
    }
    return 0;
}


// page start from 1
int history_UI(int tid, int page_in) {
    int page = page_in;
    while (true) {
        vector<chat> chat_his;
        get_chats_from_map_by_id(chat_his, tid);

        friend_client tar;
        get_friend_from_map_by_id(tid, &tar);
        system("clear");
        cout << "==========================" << endl;
        cout << "chatting history with " << tar.get_name() << endl;
        cout << "--------------------" << endl;

        sort(chat_his.begin(), chat_his.end(), chat_order_dec);

        int page_all = (chat_his.size() + PAGE_SIZE - 1) / PAGE_SIZE;
        int page_begin = PAGE_SIZE * (page - 1), page_end = PAGE_SIZE * page;
        auto c_begin = max(chat_his.begin(), chat_his.end() - page_end);
        auto c_end = chat_his.end() - page_begin;
        while (c_begin != c_end) {
            string sender_name;
            if (c_begin->get_sender() == user_id) {
                sender_name = user_name;
            } else if (c_begin->get_sender() == tid) {
                sender_name = tar.get_name();
            }
            cout << c_begin->get_time() << "  " << sender_name << endl << "  -" << c_begin->get_content() << endl;
            c_begin++;
        }
        cout << "==========================" << endl;
        cout << "  10 messages per page" << endl;
        cout << "  \'/f\' to see the next page" << endl;
        cout << "  \'/b\' to see the last page" << endl;
        cout << "  \'/e\' to exit history mode" << endl;
        cout << "  current page: " << page_all - page + 1 << ", total page: " << page_all << endl;
        cout << "==========================" << endl;
        cout << "[--history--]" << endl;
        cout << "==========================" << endl;
        string chat_buf;
        getline(cin, chat_buf);

        if (chat_buf == "/b") {
            int new_p = page + 1;
            if (new_p > page_all) {
                cout << "no more page before!" << endl;
                sleep(1);
            } else {
                page = new_p;
            }
            continue;
        } else if (chat_buf == "/e") {
            break;
        } else if (chat_buf == "/f") {
            int new_p = page - 1;
            if (new_p <= 0) {
                cout << "no more page after!" << endl;
                sleep(1);
                continue;
            } else {
                page = new_p;
                continue;
            }
        } else if (chat_buf.empty()) {
            continue;
        } else {
            cout << "wrong input!" << endl;
            sleep(1);
        }
    }
    return 0;
}