//
// Created by kumo on 5/8/21.
//

#ifndef PEKCHAT_SQL_COMMON_H
#define PEKCHAT_SQL_COMMON_H

void mysql_with_error (const char *m);

int sql_init();

int my_mysql_query (const char *q);

#endif //PEKCHAT_SQL_COMMON_H
