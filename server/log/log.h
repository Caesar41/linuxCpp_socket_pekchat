//
// Created by kumo on 5/7/21.
//

#ifndef PEKCHAT_LOG_H
#define PEKCHAT_LOG_H

#include <cstdio>

#define LOG_BUF_SIZE 1024

class log {
    char file_name[100];
public:
    log();
    int init();
    void write_error(const char *in);
    void write_log(const char *in);
    void write_log_f(const char *in);
};

#endif //PEKCHAT_LOG_H