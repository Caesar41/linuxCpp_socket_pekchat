//
// Created by kumo on 5/9/21.
//

#include <mysql/mysql.h>
#include <cstdio>
#include <cstdlib>
#include "sql_common.h"
#include "../log/log.h"
#include <cstring>
#include "friend_data.h"

extern MYSQL *con;
extern log log_file;
extern char log_buf[LOG_BUF_SIZE];
extern char sqls[1024];

int get_friends_from_database(map<int, friend_server> &m) {
    log_file.write_log("load users from database");

    sprintf(sqls, "SELECT * FROM account");
    if (my_mysql_query(sqls)) {
        mysql_with_error("select failed");
    }

    MYSQL_RES *result = mysql_store_result(con);

    if (result == NULL) {
        mysql_with_error("error happen when select");
    }

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        int uid = atoi(row[0]);
        m[uid] = friend_server(uid, row[1], false);

        sprintf(log_buf, "load user, id %d, name %s", uid, row[1]);
    }

    log_file.write_log("load users from database successfully");
    mysql_free_result(result);
}