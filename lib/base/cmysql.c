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
			arrayField[i] = (tableField *) malloc(sizeof(tableField) * 1);
			arrayField[i]->field_name = (char *) malloc(
					sizeof(char) * (sizeof(field->name) + 1));
			strcpy(arrayField[i]->field_name, field->name);
			arrayField[i]->field_type = field->type;
			arrayField[i]->field_length = (int) field->length;
			i++;
		}
	}
	*table_field = arrayField;
	return field_cnt;
}

const void *insertData(char *table, char **field_tables, int field_cnt) {
	MYSQL *conn = initMysql();
	user_data **userData;
	int field_name_length = 0;
	int field_value_length = 0;
	char *insert = "INSERT INTO";
	mallocArray(&userData, field_tables, field_cnt, &field_name_length,
			&field_value_length);
	char *field_name_sql = (char *) malloc(
			sizeof(char) * (field_name_length + 20));
	char *field_value_sql = (char *) malloc(
			sizeof(char) * (field_value_length + 20));
	char *sql = (char *) malloc(field_value_length + field_name_length + 100);
	compositeSql(&userData, field_name_sql, field_value_sql, field_cnt,
			field_name_length, field_value_length);
	sprintf(sql, "%s `%s` %s VALUES %s; ", insert, table, field_name_sql,
			field_value_sql);
	if (conn) {
		if (mysql_query(conn, "set names utf8")) {
			return mysql_error(conn);
		}
		if (mysql_query(conn, sql)) {
			return mysql_error(conn);
		}
	}
	free(field_name_sql);
	free(field_value_sql);
	free(sql);
	free_user_data(&userData, field_cnt);
	return __FUNCTION__;
}
const void *updateData(char *table, void ***data, void ***where) {
	return __FUNCTION__;
}
const void *deleteData(char *table, void ***where) {
	return __FUNCTION__;
}
const void *selectData(char *table, void ***fields, void ***where) {
	return __FUNCTION__;
}

//初始化MYSQL实例
static MYSQL *initMysql() {
	MYSQL *conn = mysql_init(0);
	if (conn == NULL) {
		return NULL;
	}
	if (mysql_real_connect(conn, "localhost", "root", "root", "test", 0,
	NULL, 0) == NULL) {
		return NULL;
	}
	return conn;
}

//执行SQL语句
const char * executeQuery(char *sql) {
	MYSQL *con = initMysql();
	if (con) {
		if (mysql_query(con, "set names utf8")) {
			return mysql_error(con);
		}
		if (mysql_query(con, sql)) {
			return mysql_error(con);
		}
	}
	return "executeQuery";
}
