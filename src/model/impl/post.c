#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>
#include "../interface/post.h"
#include "../../../lib/cgic/cgic.h"
#include "../../../lib/ctemplate/ctemplate.h"

//获取指定表的字段
int getModelField(const char *tableName, tableField ***table_field) {
	tableField **arrayField;
	MYSQL *con = initMysql();
	unsigned int field_cnt = 0;
	if (con) {
		MYSQL_FIELD *field;
		MYSQL_RES *result = mysql_list_fields(con, tableName, NULL);
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

//初始化MYSQL实例
static MYSQL *initMysql() {
	MYSQL *con = mysql_init(0);
	if (con == NULL) {
		return NULL;
	}
	if (mysql_real_connect(con, "localhost", "root", "root", "test", 0,
	NULL, 0) == NULL) {
		return NULL;
	}
	return con;
}
