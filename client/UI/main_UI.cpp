//
// Created by kumo on 5/9/21.
//

#include "main_UI.h"
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
#include "chat_UI.h"


extern int user_id;
extern string user_name;

map<int, friend_client> friends_got;

int main_UI_frame(const string& msg, const string& func) {
    system("clear");

    get_friends_from_map(friends_got);

    cout << "==========================" << endl;
    cout << "Welcome to PEKCHAT room, " << user_name << "!" << endl;
    cout << "--------------------" << endl;
    cout << "+online user:" << endl;
    auto it = friends_got.begin();
    while (it != friends_got.end()) {
        if (it->second.is_online() && it->second.get_id() != user_id) {
            cout << "  -" << it->second.get_name() << endl;
        }
        it++;
    }
    cout << "--------------------" << endl;
    cout << "+offline user:" << endl;
    it = friends_got.begin();
    while (it != friends_got.end()) {
        if (!(it->second.is_online()) && it->second.get_id() != user_id) {
            cout << "  -" << it->second.get_name() << endl;
        }
        it++;
    }
//    cout << "--------------------" << endl;
//    cout << "+group:" << endl;
//    cout << "  " << "-room314" << endl;
//    cout << "  " << "-300group" << endl;
    cout << "==========================" << endl;
    cout << "  1. chat" << endl;
    cout << "  -. group" << endl;
    cout << "  -. add friend" << endl;
    cout << "  -. handle friend request" << endl;
    cout << "  -. play gobang" << endl;
    cout << "  2. logout" << endl;
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

int main_UI_main() {
    string msg = "Please input the number for next step, or press enter to refresh user list";
    string func = "top";
    while (true) {
        main_UI_frame(msg, func);
        string op;
        getline(cin, op);
        if (op == "1") {
            func = "top";
            printf("please input the friend's name to char with: ");
            string fn;
            cin >> fn;
            int fid = 0;

            map<int, friend_client>::iterator it = friends_got.begin();
            while(it != friends_got.end()) {
                if (it->second.get_name() == fn) {
                    fid = it->second.get_id();
                    break;
                }
                it++;
            }

            if (fid == 0) {
                func = "top";
                printf("user name not exists, enter chat room failed\n");
                sleep(1);
                continue;
            } else if (fid == user_id) {
                func = "top";
                printf("you can't chat with yourself\n");
                sleep(1);
                continue;
            }

            chat_UI_main(fid);

        } else if (op == "2") {
            logout();
            cout << "Logout!" << endl;
            sleep(1);
            break;
        } else if (op.empty()) {
            msg = "Please input the number for next step, or press enter to refresh user list";
            continue;
        }
        else {
            printf("input format error, please input again\n");
            sleep(1);
            continue;
        }
    }
    return 0;
}