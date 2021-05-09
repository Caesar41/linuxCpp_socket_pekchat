//
// Created by kumo on 5/7/21.
//

#include "connect.h"
#include "log/log.h"
#include <map>
#include "data/sql_common.h"
#include "service/handler_server.h"
#include <pthread.h>
#include <csignal>
#include <unistd.h>
#include "package/client_sock.h"
#include <cstring>


log log_file;
char log_buf[LOG_BUF_SIZE] = {};

int listen_fd;
int epoll_fd;
pthread_t handler_thread;

void safe_exit(int sig) {
    log_file.write_log("now exit the server");
    close(listen_fd);
    log_file.write_log("listen_fd closed");
    close(epoll_fd);
    log_file.write_log("epoll_fd closed");
    close_client_sock();
    log_file.write_log("client sock closed");
    pthread_cancel(handler_thread);
    log_file.write_log("thread canceled");
    log_file.write_log("exit successfully");
}


int main() {

    struct sigaction sigact;
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_handler = safe_exit;
    sigaction(2, &sigact, NULL);
    sigaction(15, &sigact, NULL);

    log_file = log();

    if (log_file.init() != 0) {
        printf("log file init failed\n");
        return -1;
    }

    if (sql_init() < 0) {
        log_file.write_error("init sql failed");
        return -1;
    }

    listen_fd = init_listen_socket();
    if (listen_fd < 0) {
        log_file.write_error("init listen sock failed");
        return -1;
    }
    if (pthread_create(&handler_thread, NULL, thread_message_handler, NULL) != 0) {
        log_file.write_error("handler thread create failed");
        return -1;
    }


    // this will block the program
    run_epoll(listen_fd, &epoll_fd);

    log_file.write_log("server exit safely");

    return 0;
}
