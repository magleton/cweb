/*
 * base.c
 *
 *  Created on: 2015年11月18日
 *      Author: macro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>

#include "../cgic/cgic.h"
#include "base.h"

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

//组合SQL语句
static void *compositeSql(user_data ***data, char *field_name_sql,
		char *field_value_sql, int field_cnt, int field_name_length,
		int field_value_length) {
	user_data **userData = NULL;
	int i, k = 0;
	char *comma = ",";
	char *left_brackets = "(";
	char *right_brackets = "";
	userData = *data;
	char temp_name_sql[2048] = { 0 };
	char temp_value_sql[2048] = { 0 };
	char tmp_name[2048] = { 0 };
	char tmp_value[2048] = { 0 };
	for (i = 0; i < field_cnt; ++i) {
		if (++k == field_cnt) {
			comma = "";
			right_brackets = ")";
		}
		if (i > 0) {
			left_brackets = "";
		}
		sprintf(tmp_name, "%s`%s`%s%s", left_brackets, userData[i]->field_name,
				comma, right_brackets);
		sprintf(tmp_value, "%s'%s'%s%s", left_brackets,
				userData[i]->field_value, comma, right_brackets);
		strcat(temp_name_sql, tmp_name);
		strcat(temp_value_sql, tmp_value);
	}
	strcpy(field_name_sql, temp_name_sql);
	strcpy(field_value_sql, temp_value_sql);
	return "success";
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

//为表单字段分配内存
static int mallocArray(user_data ***ptr, char **field_tables, int field_cnt,
		int *field_name_length, int *field_value_length) {
	user_data **temp;
	int i;
	temp = (user_data **) malloc(sizeof(user_data *) * field_cnt);
	int value_length = 0;
	int name_length = 0;
	char *temp_content = NULL;
	char *temp_field_name = NULL;
	if (temp) {
		for (i = 0; i < field_cnt; ++i) {
			cgiFormStringSpaceNeeded(field_tables[i], &value_length);
			name_length = strlen(field_tables[i]);
			*field_name_length += name_length;
			*field_value_length += value_length;
			temp[i] = (user_data *) malloc(sizeof(user_data));
			temp[i]->field_name = (char *) malloc(name_length + 1);
			temp[i]->field_value = (char *) malloc(value_length + 1);
			temp[i]->field_value_length = value_length;
			temp[i]->field_name_length = name_length;
			cgiFormString(field_tables[i], temp[i]->field_value, value_length);
			strncpy(temp[i]->field_name, field_tables[i],
					sizeof(char) * (name_length + 1));
			free(temp_content);
		}
	}
	*ptr = temp;
	return 1;
}

//释放为表单字段分配的内存
static void * free_user_data(user_data ***userData, int cnt) {
	assert(*userData);
	user_data **tempData = *userData;
	int i;
	for (i = 0; i < cnt; ++i) {
		free(tempData[i]->field_name);
		free(tempData[i]->field_value);
		free(tempData[i]);
	}
	free(tempData);
	return "success";
}

void * getFormData(char **field_tables, int *field_name_length,
		int *field_value_length) {

}
