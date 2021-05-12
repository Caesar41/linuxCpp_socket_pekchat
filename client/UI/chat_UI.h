//
// Created by kumo on 5/9/21.
//

#ifndef PEKCHAT_CHAT_UI_H
#define PEKCHAT_CHAT_UI_H

#include <string>

#define PAGE_SIZE 10

using namespace std;

int chat_UI_frame(int tid, const string& msg, const string& func);

int chat_UI_main(int tid);

int history_UI(int tid, int page_in);

#endif //PEKCHAT_CHAT_UI_H
