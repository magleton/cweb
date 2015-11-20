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
#include "../interface/base.h"
#include "../../../lib/base/cmysql.h"

klass_info local_base_klass = { base_init };
klass_info *base_klass = &local_base_klass;

void base_init(void) {
	if (base_klass->init) {
		base_klass->init = NULL;
		base_klass->super = NULL;
		base_klass->class_name = "base_class";
		base_klass->size = sizeof(base);
		base_klass->ctor = base_ctor;
		base_klass->dtor = base_dtor;
	}
}

void *base_ctor(base *self) {
	return self;
}

void *base_dtor(base *self) {
	return self;
}

void setFieldValue(base *self, char *field_name, int not_form, char *value,
		field_type type) {
	int length = 0;
	int name_length = strlen(field_name);
	if (not_form) {
		length = sizeof(value);
	} else {
		cgiFormStringSpaceNeeded(field_name, &length);
	}
	self->form_datas[self->current] = (form_data*) malloc(sizeof(form_data));
	self->form_datas[self->current]->field_value = (char *) malloc(
			sizeof(char) * (length + 1));
	self->form_datas[self->current]->field_value_length = length;
	fprintf(cgiOut , "%s length %ld" , field_name , strlen(field_name));
	self->form_datas[self->current]->field_name = (char *) malloc(
			sizeof(char) * (name_length + 1));
	strncpy(self->form_datas[self->current]->field_name, field_name  ,name_length + 1);
	self->form_datas[self->current]->field_name_length = name_length;
	if (not_form) {
		strcpy(self->form_datas[self->current]->field_value, value);
	} else {
		cgiFormString(field_name, self->form_datas[self->current]->field_value,
				length);
	}
	self->field_tables[self->current] = (char *) malloc(
			sizeof(char) * (strlen(field_name) + 10));
	strncpy(self->field_tables[self->current], field_name,
			strlen(field_name) + 1);
#ifdef DEBUG
	fprintf(cgiOut, " <h1>file = %s , %s = %s , origin = %s</h1>",
			__FILE__,
			self->field_tables[self->current],
			self->form_datas[self->current]->field_name,
			field_name
	);
#endif
	self->current++;
}

form_data *getFieldValue(base *self, char *field_name) {
	int i;
	for (i = 0; i < self->field_count; i++) {
		if (strcmp(self->field_tables[i], field_name) == 0) {
			return self->form_datas[i];
		}
	}
	return NULL;
}

//组合SQL语句
static void *compositeSql(base *parent, char *field_name_sql,
		char *field_value_sql) {
	int i, k = 0;
	char *comma = ",";
	char *left_brackets = "(";
	char *right_brackets = "";
	char temp_name_sql[1024] = { 0 };
	char temp_value_sql[1024] = { 0 };
	char tmp_name[512] = { 0 };
	char tmp_value[512] = { 0 };
	for (i = 0; i < parent->field_count; ++i) {
		if (++k == parent->field_count) {
			comma = "";
			right_brackets = ")";
		}
		if (i > 0) {
			left_brackets = "";
		}
		sprintf(tmp_name, "%s`%s`%s%s", left_brackets,
				parent->form_datas[i]->field_name, comma, right_brackets);
		sprintf(tmp_value, "%s'%s'%s%s", left_brackets,
				parent->form_datas[i]->field_value, comma, right_brackets);
		//fprintf(cgiOut , "%s , %s" , tmp_name,tmp_value);
		fprintf(cgiOut , "<span style='color:green;'>%s</span>" , parent->field_tables[i]);
		strcat(temp_name_sql, tmp_name);
		strcat(temp_value_sql, tmp_value);
	}
	strcpy(field_name_sql, temp_name_sql);
	strcpy(field_value_sql, temp_value_sql);
	fprintf(cgiOut , "%s , %s" , field_name_sql,field_value_sql);
	return "success";
}

int insertData(base *parent, char *table) {
	MYSQL *conn = initMysql();
	char field_name_sql[1048] = { 0 };
	char field_value_sql[1048] = { 0 };
	int field_name_length = 0;
	int field_value_length = 0;
	char *insert = "INSERT INTO";
	char sql[2048] = { 0 };
	compositeSql(parent, field_name_sql, field_value_sql);
	sprintf(sql, "%s `%s` %s VALUES %s; ", insert, table, field_name_sql,
			field_value_sql);
	if (conn) {
		if (mysql_query(conn, "set names utf8")) {
			fprintf(cgiOut, "%s", mysql_error(conn));
			return 2;
			//return mysql_error(conn);
		}
		if (mysql_query(conn, sql)) {
			fprintf(cgiOut, "%s", mysql_error(conn));
			return 2;
			//return mysql_error(conn);
		}
	}
	int last_id = mysql_insert_id(conn);
	fprintf(cgiOut, "%s", sql);
	return last_id;
}

//转换数字到字符串
void tostring(char str[], int num) {
	int i, rem, len = 0, n;

	n = num;
	while (n != 0) {
		len++;
		n /= 10;
	}
	for (i = 0; i < len; i++) {
		rem = num % 10;
		num = num / 10;
		str[len - (i + 1)] = rem + '0';
	}
	str[len] = '\0';
}

char *random_string(char *dest) {
	size_t len = 0;
	char *p = dest;
	int three_in_a_row = 0;
	int arr[128] = { 0x0 };
	if (!srand_called) {
		srandom(time(NULL));
		srand_called = 1;
	}
	for (len = 6 + rand() % 3; len; len--, p++) {
		char *q = dest;
		*p = (rand() % 2) ? rand() % 26 + 97 : rand() % 10 + 48;
		p[1] = 0x0;
		arr[*p]++;
		if (arr[*p] == 3) {
			for (q = dest; q[2] > 0 && !three_in_a_row; q++)
				if (*q == q[1] && q[1] == q[2])
					three_in_a_row = 1;
		}
		if (three_in_a_row || arr[*p] > 3)
			return random_string(dest);
	}
	return dest;
}
