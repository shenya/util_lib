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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>

#include "king_mysql.h"

char *host = "localhost";
char *user = "root";
char *passwd = "sgy2017";
char *db = "test";
int port = 3306;

int main(void)
{
    int ret = 0;
    char query_buf[256] = {0};
    king_result_t result;
    MYSQL_ROW row;
    king_mysql_t mysql_info;
    int i = 0;
    long out_id = 0;

    memset(&mysql_info, 0, sizeof(mysql_info));
    memset(&result, 0, sizeof(result));

    ret = king_mysql_init(&mysql_info, host, port, user, passwd, db);
    if (ret < 0)
    {
        printf("%s: mysql init failed\n", __FUNCTION__);
        exit(1);
    }

    snprintf(query_buf, sizeof(query_buf), "select * from student");
    ret = king_mysql_query_result(&mysql_info, &result, query_buf, strlen(query_buf));
    //ret = king_mysql_query(&mysql_info, &result, query_buf, strlen(query_buf));
    if (ret < 0)
    {
        printf("%s: mysql query failed\n", __FUNCTION__);
        exit(1);
    }

    printf("%s: total: %d\n", __FUNCTION__, result.total);

    for (i = 0; i < result.total; i++)
    {
        printf("%s: result[%d]: %s\n", __FUNCTION__, i, result.result_set[i].s_value);
    }    

    free(result.result_set);

    snprintf(query_buf, sizeof(query_buf), "insert into student(name) values('shengy-2')");
    king_mysql_add(&mysql_info, query_buf, strlen(query_buf));

    snprintf(query_buf, sizeof(query_buf), "insert into student(name) values('shengy-3')");
    king_mysql_extend_add(&mysql_info, query_buf, strlen(query_buf), &out_id);

    printf("%s: insert get id: %ld\n", __FUNCTION__, out_id);
    king_mysql_close(&mysql_info);
    printf("hello world\n");


#if 0
    conn = mysql_init(NULL);

    conn = mysql_real_connect(conn, host, user, passwd, db, port, NULL, 0);
    if (NULL == conn)
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n",
                  mysql_error(conn));
    }

    snprintf(query_buf, sizeof(query_buf), "select * from student");
    result = mysql_real_query(conn, query_buf, strlen(query_buf));
    if (result)
    {
        printf("%s: real query failed: ret:%d\n", __FUNCTION__, ret);
        exit(1);
    }

    res = mysql_use_result(conn);

    while ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("id : %s, name: %s\n", row[0], row[1]);
    }
    mysql_free_result(res); 
    printf("hello world\n");

    mysql_close(conn);
    #endif
    return 0;
}
