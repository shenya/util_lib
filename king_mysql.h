/*
* Copyright (C) 2017 shengy (nucsgy@163.com)

* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef __KING_MYSQL_H__
#define __KING_MYSQL_H__

#include <mysql/mysql.h>

#define KING_COLUME_LENGTH_MAX 1024

typedef struct king_mysql_s
{
    MYSQL *conn;
    int connect_flag;
    
    char host[32];    
    int port;
    char conn_path[64];
    char user[32];
    char passwd[32];
    char db_name[32];
} king_mysql_t;

typedef struct king_element_s
{
    char s_value[KING_COLUME_LENGTH_MAX];
} king_element_t;

typedef struct king_result_s
{
    int total;
    king_element_t *result_set;
} king_result_t;

int king_mysql_init(king_mysql_t *info, char *host, int port,
        char *user, char *passwd, char *db_name);

int king_mysql_connect(king_mysql_t *info);

int king_mysql_close(king_mysql_t *info);

int king_mysql_query(king_mysql_t *info, MYSQL_RES **out_result,
        char *sql_buf, int len);

int king_mysql_query_result(king_mysql_t *info, king_result_t *out_result,
        char *sql_buf, int len);

#endif
