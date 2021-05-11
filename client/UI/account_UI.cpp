//
// Created by kumo on 5/7/21.
//

#include "account_UI.h"
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include "../function/account_function.h"
#include "../package/simple_code.h"
#include "../function/container_client.h"
#include "../package/message.h"
#include "main_UI.h"

using namespace std;

extern int user_id;
extern string user_name;

int login_UI_frame(const string &msg, const string &func) {
    system("clear");

    cout << "==========================" << endl;
    cout << "Welcome to PEKCHAT room!" << endl;
    cout << "  1. login" << endl;
    cout << "  2. register" << endl;
    cout << "  3. exit" << endl;
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

int login_UI_main() {
    string msg = "Please input the number for next step";
    string func = "top";
    while (true) {
        login_UI_frame(msg, func);
        string op;
        getline(cin, op);
        if (op == "1") {
            func = "login";
            msg = "";
            // login
            while (true) {
                login_UI_frame(msg, func);
                string username, password;
                cout << "input your username: ";
                cin >> username;
                if (username.size() >= 20) {
                    msg = "username too long, please input again";
                    continue;
                }
                cout << "input your password: ";
                cin >> password;
                if (password.size() >= 20 || password.size() < 6) {
                    msg = "password too long or too short, please input again";
                    continue;
                }
                encryption(password);
                if (login_request(username.c_str(), password.c_str()) < 0) {
                    msg = "login request failed";
                }
                message *m = new message;
                get_response(LOGIN_RESPONSE, m);
                int r = m->get_int(STATE);
                if (r == 0) {
                    printf("user %s login successfully\n", username.c_str());

                    user_id = m->get_int(ID_TOKEN);
                    user_name = username;
                    sleep(1);
                    login_OK();

                    printf("init the chat room...\n");
                    get_response(INIT_COMPLETE, m);
                    printf("init complete!\n");
                    sleep(1);
                    main_UI_main();

                    // logout
                    msg = "logout successfully";
                    func = "top";

                } else if (r == -1) {
                    printf("login failed, user not exists\n");
                    sleep(1);
                } else if (r == -2) {
                    printf("login failed, wrong password\n");
                    sleep(1);
                } else if (r == -3) {
                    printf("login failed, this user is online already\n");
                    sleep(1);
                } else {
                    printf("login failed, server error\n");
                    sleep(1);
                }
                break;
            }
        } else if (op == "2") {
            func = "register";
            msg = "";
            while (true) {
                login_UI_frame(msg, func);
                string username, password, pw2;
                cout << "input the username (length < 20, and NEVER contains space): ";
                cin >> username;
                if (username.size() >= 20) {
                    msg = "username too long, please input again";
                    continue;
                }
                cout << "input the password (6 <= length < 20, and NEVER contains space): ";
                cin >> password;
                if (password.size() >= 20 || password.size() < 6) {
                    msg = "password too long or too short, please input again";
                    continue;
                }
                cout << "input the password again: ";
                cin >> pw2;
                if (password != pw2) {
                    msg = "different from before, please input again";
                    continue;
                }
                encryption(password);
                if (register_request(username.c_str(), password.c_str()) < 0) {
                    printf("register failed\n");
                }
                message *m = new message;
                get_response(REGISTER_RESPONSE, m);
                int r = m->get_int(STATE);
                if (r > 0) {
                    printf("register successfully\n");
                    sleep(1);
                } else if (r == -2) {
                    printf("register failed, username exists\n");
                    sleep(1);
                } else {
                    printf("register failed, server error\n");
                    sleep(1);
                }
                break;
            }


        } else if (op == "3") {
            cout << "Thanks for using! Bye!" << endl;
            sleep(1);
            break;
        } else if (op.empty()) {
            msg = "Please input the number for next step";
            continue;
        } else {
            printf("input format error, please input again\n");
            sleep(1);
            continue;
        }
    }
    return 0;
}