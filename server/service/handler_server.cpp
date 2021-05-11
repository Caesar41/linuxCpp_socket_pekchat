//
// Created by kumo on 5/8/21.
//

#include "../log/log.h"
#include <pthread.h>
#include "account_service.h"
#include "container_server.h"
#include <time.h>
#include "../package/client_sock.h"
#include "../package/message.h"

using namespace std;

extern log log_file;
extern char log_buf[];

int init_thread() {
    if (init_containers() < 0) {
        log_file.write_error("containers init failed");
        return -1;
    }
    return 0;
}

[[noreturn]] void *thread_message_handler(void *arg) {
    message m;
    init_thread();

    log_file.write_log("handler thread init successfully");

    while (true) {
        get_message(&m);
        int t;
        int r;
        m.get_int(TYPE, &t);
        switch (t) {
            case REGISTER_REQUEST:
                r = register_func(m);
                if (r < 0) {
                    log_file.write_error("register fun failed");
                } else {
                    log_file.write_log("register fun done successfully");
                }
                break;
            case LOGIN_REQUEST:
                r = login_func(m);
                if (r < 0) {
                    log_file.write_error("login fun failed");
                } else {
                    log_file.write_log("login fun done successfully");
                }
                break;
            case LOGIN_OK:
                // this should init it at the same time
                login_ok(m);
                break;
            case LOGOUT:
                user_logout(m);
                break;
            case TIME_REQUEST:
                send_server_time(m.get_fd());
                break;
            case CHAT_SEND:
                got_chat_function(m);
                break;
        }
    }

    pthread_exit(NULL);
}
