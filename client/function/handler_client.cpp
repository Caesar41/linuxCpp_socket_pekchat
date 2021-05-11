//
// Created by kumo on 5/8/21.
//

#include <queue>
#include <pthread.h>
#include <semaphore.h>
#include "handler_client.h"
#include <cstdio>
#include <sys/socket.h>
#include <unistd.h>
#include "container_client.h"

using namespace std;


int init_thread() {

    if (init_containers() < 0) {
        return -1;
    }
    return 0;
}


int client_read(int client_fd, message *m) {
    static char buf[MESSAGE_LENGTH];
    static int buf_size = MESSAGE_LENGTH;
    ssize_t in_size;
    while (true) {
        in_size = recv(client_fd, &buf[MESSAGE_LENGTH - buf_size], buf_size, 0);
        if (in_size <= 0) {
            // client shut down
            break;
        } else {
            buf_size -= in_size;
            if (buf_size == 0) {
                buf_size = MESSAGE_LENGTH;
                // handle the message
                m->parse(buf);
                // printf("received type = %d\n", m->get_int(TYPE));
                break;
            }
        }
    }

    return (int) in_size;
}

[[noreturn]] void *thread_message_handler(void *arg) {
    message m;
    init_thread();
    int fd = (int) (long) arg;
    while (true) {
        client_read(fd, &m);
        int t;
        int r;
        m.get_int(TYPE, &t);
        switch (t) {
            case REGISTER_RESPONSE:
                add_response(m);
                break;
            case LOGIN_RESPONSE:
                add_response(m);
                break;
            case FRIEND_RECV:
                add_friend(m);
                break;
            case FRIEND_LOG_CHANGE:
                friend_log_change(m);
                break;
            case INIT_COMPLETE:
                add_response(m);
                break;
            case TIME_RESPONSE:
                add_response(m);
                break;
            case CHAT_RECV:
                add_chat(m);
                break;
        }
    }

    pthread_exit(NULL);
}
