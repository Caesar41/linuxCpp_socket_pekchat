//
// Created by kumo on 5/7/21.
//

#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include "log.h"
#include <dirent.h>
#include<sys/stat.h>
#include<sys/types.h>

log::log() {
}

int log::init() {
    char log_name[100] = {};
    getcwd(log_name, 100);

    char ptr_t[50] = {};
    time_t tm;
    time(&tm);
    struct tm *local = localtime(&tm);
    strftime(ptr_t, 50, "%Y%m%d", local);  // update log everyday

    strcat(log_name, "/logfile");

    if (access(log_name, F_OK) != 0) {
        if (mkdir(log_name, 0755) != 0) {
            return -1;
        }
    }

    strcat(log_name, "/pekchat_log-");
    strcat(log_name, ptr_t);

    strcpy(this->file_name, log_name);

    char in_time[100] = {};
    strftime(in_time, 50, "%Y/%m/%d-%H:%M:%S", local);
    strcat(in_time, "init log file successfully");
    write_log_f(in_time);
    return 0;
}

void log::write_log(const char *in) {
    FILE *p = fopen(this->file_name, "a+");

    char t[50] = {};
    time_t tm;
    time(&tm);
    struct tm *local = localtime(&tm);
    strftime(t, 50, "%Y/%m/%d-%H:%M:%S", local);

    fprintf(p, "\n%s %s", t, in);

    fclose(p);
}

void log::write_error(const char *in) {
    write_log("ERROR: ");
    write_log_f(in);
}

void log::write_log_f(const char *in) {
    FILE *p = fopen(this->file_name, "a+");

    fprintf(p, "%s", in);

    fclose(p);
}