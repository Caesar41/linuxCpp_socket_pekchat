//
// Created by kumo on 5/8/21.
//

#ifndef PEKCHAT_HANDLER_SERVER_H
#define PEKCHAT_HANDLER_SERVER_H


[[noreturn]] void *thread_message_handler(void *arg);


int init_thread();

#endif //PEKCHAT_HANDLER_SERVER_H
