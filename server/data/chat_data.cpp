//
// Created by kumo on 5/11/21.
//

#include "chat_data.h"
#include "sql_common.h"
#include <mysql/mysql.h>
#include <cstdio>
#include <cstdlib>
#include "sql_common.h"
#include "../log/log.h"
#include <cstring>
#include "../service/container_server.h"

extern MYSQL *con;
extern log log_file;
extern char log_buf[LOG_BUF_SIZE];
extern char sqls[1024];

int add_chat_to_data(chat c) {

    sprintf(sqls, "INSERT INTO chat (sender, receiver, time, content) VALUES(%d, %d, %d, \"%s\")",
            c.get_sender(), c.get_receiver(), c.get_time_int(), c.get_content().c_str());
    if (my_mysql_query(sqls)) {
        mysql_with_error("insert failed");
        return -1;
    }

//    sprintf(sqls, "SELECT last_insert_id();");
//    if (my_mysql_query(sqls)) {
//        mysql_with_error("fetch last call failed");
//        return -1;
//    }
//
//    MYSQL_RES *result = mysql_store_result(con);
//
//    if (result == NULL) {
//        mysql_with_error("error happen when select");
//    }
//    MYSQL_ROW row;
//    row = mysql_fetch_row(result);
//
//    int uid = atoi(row[0]);
//
//    mysql_free_result(result);
//
//    return uid;
    return 0;
}

int get_chats_from_database() {
    log_file.write_log("load chats from database");

    sprintf(sqls, "SELECT * FROM chat");
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
        chat c = chat(atoi(row[0]), atoi(row[1]), atoi(row[2]), row[4], atoi(row[3]));
        add_chat_to_map(c);
    }

    log_file.write_log("load chats from database successfully");
    mysql_free_result(result);
}