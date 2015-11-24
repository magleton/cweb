/*
 * cmysql.c
 *
 *  Created on: 2015年11月19日
 *      Author: macro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>

#include "../cgic/cgic.h"
#include "cmysql.h"

#define CMYSQL_HOST "127.0.0.1"
#define CMYSQL_USERNAME "root"
#define CMYSQL_PWD "root"
#define CMYSQL_DBNAME "test"

int getTableFields(const char *table_name, tableField ***table_field) {
	tableField **arrayField;
	MYSQL *con = initMysql();
	unsigned int field_cnt = 0;
	if (con) {
		MYSQL_FIELD *field;
		MYSQL_RES *result = mysql_list_fields(con, table_name, NULL);
		field_cnt = mysql_num_fields(result);
		arrayField = (tableField **) malloc(sizeof(tableField *) * field_cnt);
		int i = 0;
		while (field = mysql_fetch_field(result)) {
			//fprintf(cgiOut, "field_cnt= %d Field =%s Type=%d Length=%ld<br/>", field_cnt , field->name, field->type, field->length);
			arrayField[i] = (tableField *) malloc(sizeof(tableField));
			arrayField[i]->field_name = (char *) malloc(
					sizeof(char) * (sizeof(field->name) + 1));
			strcpy(arrayField[i]->field_name, field->name);
			arrayField[i]->field_type = field->type;
			arrayField[i]->field_length = field->length;
			i++;
		}
	}
	fprintf(cgiOut, "%d", field_cnt);
	*table_field = arrayField;
	return field_cnt;
}

//初始化MYSQL实例
MYSQL *initMysql() {
	MYSQL *conn = mysql_init(0);
	if (conn == NULL) {
		return NULL;
	}
	if (mysql_real_connect(conn, CMYSQL_HOST, CMYSQL_USERNAME, CMYSQL_PWD,
	CMYSQL_DBNAME, 0,
	NULL, 0) == NULL) {
		return NULL;
	}
	return conn;
}

//执行SQL语句(增加，删除，更新)
const char * executeQueryWithoutResult(char *sql) {
	MYSQL *con = initMysql();
	if (con) {
		if (mysql_query(con, "set names utf8")) {
			finish_with_error(con);
		}
		if (mysql_query(con, sql)) {
			finish_with_error(con);
		}
	}
	return "executeQuery";
}

//执行SQL语句(查询)
MYSQL * executeQueryWithResult(char *sql) {
	MYSQL *con = initMysql();
	if (con) {
		if (mysql_query(con, "set names utf8")) {
			finish_with_error(con);
		}
		if (mysql_query(con, sql)) {
			finish_with_error(con);
		}
	}
	return con;
}

//显示错误
void finish_with_error(MYSQL *con) {
	fprintf(cgiOut, "%s\n", mysql_error(con));
	mysql_close(con);
	exit(1);
}
