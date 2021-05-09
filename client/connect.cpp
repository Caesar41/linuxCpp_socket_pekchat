//
// Created by kumo on 5/7/21.
//

#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>

int init_client_sock() {
    // when test, ip is 127.0.0.1, port is 5005
    // client has no log
    // now, because we have no GUI, so all printf is not formal

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        printf("Socket failed\n");
        return -1;
    }

    struct sockaddr_in sock_addr;
    sock_addr.sin_port = htons(5005);
    sock_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sock_addr.sin_family = AF_INET;

    if (connect(sock_fd, (sockaddr *) &sock_addr, sizeof(sock_addr)) < 0) {
        printf("Connect failed\n");
        close(sock_fd);
        return -1;
    }

    printf("Client sock init successfully\n");

    return sock_fd;

}