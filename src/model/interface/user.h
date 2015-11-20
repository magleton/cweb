/*
 * user.h
 *
 *  Created on: 2015年11月18日
 *      Author: macro
 */

#ifndef SRC_MODEL_INTERFACE_USER_H_
#define SRC_MODEL_INTERFACE_USER_H_

#include "../../../lib/oop/lw_new.h"
#include "base.h"

extern klass_info *user_klass;

typedef struct _user user;
struct _user {
	base parent;
};

void user_init(void);
void user_set_field_value(user *self, char *field_name, int not_form,
		char *value , field_type type);
form_data *user_get_field_value(user *self, char *field_name);

void *user_ctor(user *user);
void *user_dtor(user *user);

static void setFieldValue1(user *self, char *field_name);
static form_data *getFieldValue1(user *self, char *field_name);

#endif /* SRC_MODEL_INTERFACE_USER_H_ */
