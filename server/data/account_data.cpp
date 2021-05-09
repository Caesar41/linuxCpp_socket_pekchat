//
// Created by kumo on 5/8/21.
//

#include <mysql/mysql.h>
#include <cstdio>
#include <cstdlib>
#include "sql_common.h"
#include "../log/log.h"
#include <cstring>

extern MYSQL *con;
extern log log_file;
extern char log_buf[LOG_BUF_SIZE];
extern char sqls[1024];


// return 0 if success, -2 if username repeated, other if other sql error
int register_data(const char *username, const char *password) {

    sprintf(sqls, "SELECT * FROM account WHERE username=\"%s\"", username);
    if (my_mysql_query(sqls)) {
        mysql_with_error("select failed");
    }

    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL) {
        mysql_with_error("error happen when select");
    }
    if (mysql_fetch_row(result)) {
        return -2;
    }
    mysql_free_result(result);

    sprintf(sqls, "INSERT INTO account (username, password) VALUES(\"%s\",\"%s\")", username, password);
    if (my_mysql_query(sqls)) {
        mysql_with_error("insert failed");
        return -1;
    }

    sprintf(sqls, "SELECT * FROM account WHERE username=\"%s\"", username);

    if (my_mysql_query(sqls)) {
        mysql_with_error("select failed");
    }

    result = mysql_store_result(con);
    if (result == NULL) {
        mysql_with_error("error happen when select");
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(result);

    int uid = atoi(row[0]);

    mysql_free_result(result);

    return uid;
}


// return 0 if success, -1 if username not exists, -2 if password wrong, -3 if other sql error, > 0 id if success
int login_data(const char *username, const char *password) {

    sprintf(sqls, "SELECT * FROM account WHERE username=\"%s\"", username);
    if (my_mysql_query(sqls)) {
        mysql_with_error("select failed");
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        mysql_with_error("error happen when select");
    }

    MYSQL_ROW row;

    row = mysql_fetch_row(result);

    if (row == NULL) {
        return -1;
    }

    if (strcmp(row[2], password) != 0) {
        return -2;
    }

    int id = atoi(row[0]);

    mysql_free_result(result);
    return id;
}