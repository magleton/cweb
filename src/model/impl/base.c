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

void setFieldValue(base *self, char *field_name, int not_form, char *value) {
	int length = 0;
	if (not_form) {
		length = sizeof(value);
	} else {
		cgiFormStringSpaceNeeded(field_name, &length);
	}
	self->form_datas[self->current] = (form_data*) malloc(sizeof(form_data));
	self->form_datas[self->current]->field_value = (char *) malloc(
			sizeof(char) * (length + 1));
	self->form_datas[self->current]->field_value_length = length;
	self->form_datas[self->current]->field_name = (char *) malloc(
			sizeof(char) * (sizeof(field_name) + 1));
	strncpy(self->form_datas[self->current]->field_name, field_name,
			sizeof(field_name));
	self->form_datas[self->current]->field_name_length = sizeof(field_name);
	if (not_form) {
		strcpy(self->form_datas[self->current]->field_value, value);
	} else {
		cgiFormString(field_name, self->form_datas[self->current]->field_value,
				length);
	}
	self->field_tables[self->current] = (char *) malloc(
			sizeof(char) * (sizeof(field_name) + 10));
	strncpy(self->field_tables[self->current], field_name, sizeof(field_name));
#ifdef DEBUG
	fprintf(cgiOut, " <h1>field = %s , self->field_tables[%d] = %s , %p</h1>",
			self->form_datas[self->current]->field_value, self->current,
			self->field_tables[self->current],
			self->field_tables[self->current]);
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
	char temp_name_sql[2048] = { 0 };
	char temp_value_sql[2048] = { 0 };
	char tmp_name[2048] = { 0 };
	char tmp_value[2048] = { 0 };
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
		strcat(temp_name_sql, tmp_name);
		strcat(temp_value_sql, tmp_value);
	}
	strcpy(field_name_sql, temp_name_sql);
	strcpy(field_value_sql, temp_value_sql);
	return "success";
}

const void *insertData1(base *parent, char *table) {
	MYSQL *conn = initMysql();
	char field_name_sql[2048] = { 0 };
	char field_value_sql[2048] = { 0 };
	int field_name_length = 0;
	int field_value_length = 0;
	char *insert = "INSERT INTO";
	char sql[2048] = { 0 };
	compositeSql(parent, field_name_sql, field_value_sql);
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
	fprintf(cgiOut, "%s", sql);
	return __FUNCTION__;
}
