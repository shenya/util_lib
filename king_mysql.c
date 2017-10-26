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

int king_mysql_init(king_mysql_t *info, char *host, int port,
        char *user, char *passwd, char *db_name)
{
    if (NULL == info)
    {
        printf("%s: Invalid param\n", __FUNCTION__);
        return -1;
    }

    strcpy(info->host, host);
    info->port = port;
    strcpy(info->user, user);
    strcpy(info->passwd, passwd);
    strcpy(info->db_name, db_name);

    return 0;
}

int king_mysql_connect(king_mysql_t *info)
{
    info->conn = mysql_init(NULL);
    if (NULL == info->conn)
    {
        printf("%s: mysql connect failed\n", __FUNCTION__);
        return -1;
    }

    info->conn = mysql_real_connect(info->conn, info->host, info->user,
            info->passwd, info->db_name, info->port, NULL, 0);
    if (NULL == info->conn)
    {
        fprintf(stderr, "Failed to connect to database: Error: %s\n",
                  mysql_error(info->conn));
        return -1;
    }

    return 0;

}

int king_mysql_close(king_mysql_t *info)
{
    if (NULL == info)
    {
        printf("%s: Invalid param\n", __FUNCTION__);
        return -1;
    }

    if (NULL == info->conn)
    {
        return 0;
    }

    mysql_close(info->conn);
    info->conn = NULL;

    return 0;

}

int king_mysql_query(king_mysql_t *info, MYSQL_RES **out_result,
        char *sql_buf, int len)
{
    int ret = 0;
    MYSQL_RES *result = NULL;

    if (NULL == info)
    {
        printf("%s: Invalid param", __FUNCTION__);
        return -1;
    }

    if (NULL == info->conn)
    {
        ret = king_mysql_connect(info);
        if (ret < 0)
        {
            printf("%s: mysql connect failed", __FUNCTION__);
            return -1;
        }
    }
    ret = mysql_real_query(info->conn, sql_buf, strlen(sql_buf));
    if (ret)
    {
        printf("%s: query failed:ret:%d\n", __FUNCTION__, ret);
        return -1;
    }

    result = mysql_store_result(info->conn);
    if (NULL == result)
    {
        printf("%s: mysql use result failed\n", __FUNCTION__);
        return -1;
    }

    *out_result = result;

    return 0;
}

int king_mysql_query_result(king_mysql_t *info, king_result_t *out_result,
        char *sql_buf, int len)
{
    int ret = 0;
    int status = 0;
    MYSQL_RES *result = NULL;
    king_element_t *p_element = NULL;
    int row_num = 0;
    int field_num = 0;
    int i = 0;
    MYSQL_ROW row;

    if (NULL == info || NULL == out_result || NULL == sql_buf)
    {
        printf("%s: Invalid param", __FUNCTION__);
        return -1;
    }

    ret = king_mysql_query(info, &result, sql_buf,len);
    if (ret < 0)
    {
        printf("%s: Invalid param", __FUNCTION__);
        return -1;
    }

    row_num = mysql_num_rows(result);
    field_num = mysql_num_fields(result);

    printf("%s: row num: %d, field num: %d\n", __FUNCTION__, row_num, field_num);
    out_result->result_set = (king_element_t *)malloc(sizeof(king_element_t) * row_num *
            field_num);
    if (NULL == out_result->result_set)
    {
        printf("%s: malloc failed", __FUNCTION__);
        status = -1;
        goto on_return;
    }

    p_element = (king_element_t *)out_result->result_set;
    while (1)
    {
        row = mysql_fetch_row(result);
        if (NULL == row)
        {
            break;
        }

        for (i = 0; i < field_num; i++)
        {
            snprintf(p_element->s_value, KING_COLUME_LENGTH_MAX - 1, "%s",
                    row[i]?row[i]:"");
            out_result->total++;
            p_element++;
        }
    }

on_return:
    mysql_free_result(result); 

    return status;
}

int king_mysql_add(king_mysql_t *info,
        char *sql_buf, int len)
{
    int ret = 0;
    int status = 0;

    if (NULL == info || NULL == sql_buf)
    {
        printf("%s: Invalid param", __FUNCTION__);
        return -1;
    }

    if (NULL == info->conn)
    {
        ret = king_mysql_connect(info);
        if (ret < 0)
        {
            printf("%s: mysql connect failed", __FUNCTION__);
            return -1;
        }
    }

    ret = mysql_real_query(info->conn, sql_buf, strlen(sql_buf));
    if (ret)
    {
        printf("%s: query failed:ret:%d\n", __FUNCTION__, ret);
        return -1;
    }

on_return:

    return status;
}

int king_mysql_extend_add(king_mysql_t *info,
        char *sql_buf, int len, long *out_id)
{
    int ret = 0;
    int status = 0;

    if (NULL == info || NULL == sql_buf)
    {
        printf("%s: Invalid param", __FUNCTION__);
        return -1;
    }

    if (NULL == info->conn)
    {
        ret = king_mysql_connect(info);
        if (ret < 0)
        {
            printf("%s: mysql connect failed", __FUNCTION__);
            return -1;
        }
    }        

    ret = mysql_real_query(info->conn, sql_buf, strlen(sql_buf));
    if (ret)
    {
        printf("%s: query failed:ret:%d\n", __FUNCTION__, ret);
        return -1;
    }

    *out_id = mysql_insert_id(info->conn);

on_return:

    return status;
}

int king_free_result(king_result_t *result)
{
    if (result)
    {
        if (result->result_set)
        {
            free(result->result_set);
            result->result_set = NULL;
        }
    }

    return 0;
}