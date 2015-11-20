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


void user_set_field_value(user *self, char *field_name , int not_form,
		char *value , field_type type) {
	setFieldValue(BASE(self), field_name, not_form, value , type);
}

form_data *user_get_field_value(user *self, char *field_name) {
	form_data *current_user_data = getFieldValue((BASE(self)), field_name);
	if (current_user_data) {
#ifdef SHOW_FIELD_KEY_VALUE
		fprintf(cgiOut, "%s = %s<br/>", current_user_data->field_name,
				current_user_data->field_value);
#endif
		return current_user_data;
	}
	return NULL;
}

void *user_ctor(user *self) {
	((void_fun) (klass_of(self)->super->ctor))(self);
	(BASE(self))->field_count = 5;
	(BASE(self))->current = 0;
	(BASE(self))->form_datas = (form_data **) malloc(sizeof(form_data *) * 5);
	(BASE(self))->field_tables = (char **) malloc(sizeof(char *) * 5);
	return self;
}

void *user_dtor(user *self) {
	(((void_fun) (klass_of(self)->super->dtor))(self));
	return self;
}
