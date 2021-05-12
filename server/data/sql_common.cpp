//
// Created by kumo on 5/8/21.
//

#include <mysql/mysql.h>
#include "../log/log.h"

extern log log_file;
extern char log_buf[LOG_BUF_SIZE];

MYSQL *con;
char sqls[1024];

void mysql_with_error (const char *m) {
    log_file.write_error(m);
    log_file.write_log_f("---error message: ");
    log_file.write_log_f(mysql_error(con));
    mysql_close(con);
}

int my_mysql_query (const char *q) {
    log_file.write_log("SQL: ");
    log_file.write_log_f(q);
    return mysql_query(con, q);
}

int sql_init() {
    con = mysql_init(NULL);

    if (con == NULL)
    {
        mysql_with_error("mysql init failed");
        return -1;
    }

    if (mysql_real_connect(con, "***", "***", "***",
                           "***", 0, NULL, 0) == NULL)
    {
        mysql_with_error("connect with database failed");
        return -1;
    }

    log_file.write_log("MySQL connected successfully");

    //mysql_close(con);
    return 0;
}