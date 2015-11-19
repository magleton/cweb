/*
 * user.c
 *
 *  Created on: 2015年11月18日
 *      Author: macro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../interface/user.h"
#include "../interface/base.h"
#include "../../../lib/oop/lw_new.h"
#include "../../../lib/cgic/cgic.h"

klass_info local_user_klass = { user_init };
klass_info *user_klass = &local_user_klass;

void user_init(void) {
	if (user_klass->init) {
		base_init();
		memcpy(user_klass, base_klass, sizeof(klass_info));
		user_klass->super = base_klass;
		user_klass->class_name = "user_klass";
		user_klass->size = sizeof(user);
		user_klass->ctor = user_ctor;
		user_klass->dtor = user_dtor;
	}
}

/*char *compositeSql1(char *table_name, user *self, char *sql) {
 char tempSql[4096] = { 0 };
 fprintf(cgiOut, "%s , %s , %s \n", user_get_username(self),
 user_get_pwd(self), user_get_salt(self));
 int i, k = 0;
 char *comma = ",";
 char *left_brackets = "(";
 char *right_brackets = "";
 sql = "";
 strcpy(sql, tempSql);
 return "success";
 }*/

void user_set_field_value(user *self, char *field_name) {
	setFieldValue((BASE(self)), field_name);
}

void user_get_field_value(user *self, char *field_name) {
	user_data *current_user_data = getFieldValue((BASE(self)), field_name);
	if (current_user_data) {
		fprintf(cgiOut, "%s = %s<br/>", current_user_data->field_name,
				current_user_data->field_value);
	}
}

void *user_ctor(user *self) {
	((void_fun) (klass_of(self)->super->ctor))(self);
	(BASE(self))->field_count = 5;
	(BASE(self))->current = 0;
	(BASE(self))->user_datas = (user_data **) malloc(sizeof(user_data *) * 5);
	return self;
}

void *user_dtor(user *self) {
	base *base_obj = (base *)(((void_fun) (klass_of(self)->super->dtor))(self));
	return self;
}

/*static void setFieldValue1(user *self, char *field_name) {
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
 }*/

/*static user_data *getFieldValue1(user *self, char *field_name) {
 int i;
 for (i = 0; i < self->field_count; i++) {
 if (strcmp(self->field_tables[i], field_name) == 0) {
 return self->user_datas[i];
 }
 }
 return NULL;
 }*/
