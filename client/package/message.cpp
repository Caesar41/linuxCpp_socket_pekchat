//
// Created by kumo on 5/7/21.
//

#include "message.h"
#include <netinet/in.h>
#include <cstring>
#include <cstdio>

message::message() {
    memset((void *)this, 0, MESSAGE_LENGTH);
}

message::message(int t) {
    memset((void *)this, 0, MESSAGE_LENGTH);
    set_int(TYPE, t);
}

// return 0 if success, -1 if fail. tar save the value
int message::set_int(int n, int value) {
    if (n < 0 || n >= INT_NUMBER) {
        return -1;
    }
    integer[n] = value;
    return 0;
}

int message::set_str(int n, const char *s) {
    if (n < 0 || n >= STR_NUMBER) {
        return -1;
    }
    if (strlen(s) > STR_LENGTH) {
        return -2;
    }
    strcpy(strs[n], s);
    return 0;
}

int message::set_content(const char *s) {
    if (strlen(s) > CONTENT_LENGTH) {
        return -2;
    }
    strcpy(content, s);
    return 0;
}

// return 0 if success, -1 if fail. tar save the value
int message::get_int(int n, int *tar) {
    if (n < 0 || n >= INT_NUMBER) {
        return -1;
    }
    *tar = integer[n];
    return 0;
}

int message::get_int(int n) {
    if (n < 0 || n >= INT_NUMBER) {
        return -12345;
    }
    return integer[n];
}

int message::set_fd(int n) {
    fd = n;
    return 0;
}

int message::get_fd() {
    return fd;
}

int message::get_str(int n, char *s) {
    if (n < 0 || n >= STR_NUMBER) {
        return -1;
    }
    strcpy(s, strs[n]);
    return 0;
}

char *message::get_str(int n) {
    if (n < 0 || n >= STR_NUMBER) {
        return NULL;
    }
    return strs[n];
}

// buf save the message in str
void message::serialize(char *buf) {
    for (int i : integer) {
        unsigned int item = htonl(i);
        memcpy(buf, &item, 4);

        buf += sizeof(int);
    }
    for (auto & str : strs) {
        memcpy(buf, str, STR_LENGTH);
        buf += STR_LENGTH;
    }
    memcpy(buf, content, CONTENT_LENGTH);
}

// source buf, target m
void message::parse(char *buf) {
    for (int & i : integer) {
        unsigned int item;
        memcpy(&item, buf, 4);
        item = ntohl(item);
        i = (int) item;
        buf += sizeof(int);
    }
    for (auto & str : strs) {
        memcpy(str, buf, STR_LENGTH);
        buf += STR_LENGTH;
    }
    memcpy(content, buf, CONTENT_LENGTH);
}