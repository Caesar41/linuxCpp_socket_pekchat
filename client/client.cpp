//
// Created by kumo on 5/7/21.
//

#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include "connect.h"
#include "UI/account_UI.h"
#include "function/handler_client.h"
#include "function/container_client.h"
#include <pthread.h>
#include "client.h"
#include <string>
#include <csignal>
#include <cstring>

using namespace std;

int sock_fd;
char send_buf[MESSAGE_LENGTH];

int user_id = 0;
string user_name = "";

pthread_t handler_thread;


long int client_send(message *m) {
    m->set_int(SOURCE, user_id);
    m->serialize(send_buf);
    return send(sock_fd, send_buf, MESSAGE_LENGTH, 0);
}

int get_server_time() {
    message time_request = message(TIME_REQUEST);
    client_send(&time_request);

    get_response(TIME_RESPONSE, &time_request);

    return time_request.get_int(TIME_SERVER);
}

void safe_exit_client(int sig) {
    if (user_id > 0) {
        message logout = message(LOGOUT);
        client_send(&logout);
    }
    close(sock_fd);
    exit(0);
}


int main() {

    struct sigaction sigact;
    memset(&sigact, 0, sizeof(sigact));
    sigact.sa_handler = safe_exit_client;
    sigaction(2, &sigact, NULL);
    sigaction(15, &sigact, NULL);

    system("clear");
    printf("Connecting to the server...\n");

    sleep(1);

    sock_fd = init_client_sock();

    sleep(1);

    if (sock_fd < 0) {
        printf("Init client sock failed\n");
        return -1;
    }

    printf("Connected successfully!\n");

    sleep(1);

    if (pthread_create(&handler_thread, NULL, thread_message_handler, (void *) (long) sock_fd) != 0) {
        printf("handler thread create failed");
        return -1;
    }

    login_UI_main();

    return 0;

}