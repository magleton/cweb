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
	fprintf(cgiOut , "%s<br/>" , __FUNCTION__);
	return self;
}

 void setFieldValue(base *self, char *field_name) {
	int length = 0;
	cgiFormStringSpaceNeeded(field_name, &length);
	self->user_datas[self->current] = (user_data*) malloc(sizeof(user_data));
	self->user_datas[self->current]->field_value = (char *) malloc(
			sizeof(char) * (length + 1));
	self->user_datas[self->current]->field_value_length = length;
	self->user_datas[self->current]->field_name = (char *) malloc(
			sizeof(char) * (sizeof(field_name) + 1));
	strncpy(self->user_datas[self->current]->field_name, field_name,
			sizeof(field_name));
	self->user_datas[self->current]->field_name_length = sizeof(field_name);
	cgiFormString(field_name, self->user_datas[self->current]->field_value,
			length);
	fprintf(cgiOut, "%s , <h1>current= %d</h1>",
			self->user_datas[self->current]->field_value, self->current);
	self->field_tables[self->current] = (char *) malloc(
			sizeof(char) * (sizeof(field_name) + 1));
	strncpy(self->field_tables[self->current], field_name, sizeof(field_name));
	self->current++;
}

 user_data *getFieldValue(base *self, char *field_name) {
	int i;
	for (i = 0; i < self->field_count; i++) {
		if (strcmp(self->field_tables[i], field_name) == 0) {
			return self->user_datas[i];
		}
	}
	return NULL;
}


//为表单字段分配内存
/*static int mallocArray(user_data ***ptr, char **field_tables, int field_cnt,
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
}*/

//释放为表单字段分配的内存
/*static void * free_user_data(user_data ***userData, int cnt) {
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
}*/

//组合SQL语句
/*static void *compositeSql(user_data ***data, char *field_name_sql,
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
}*/
