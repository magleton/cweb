/*
 * post.h
 *
 *  Created on: 2015年11月15日
 *      Author: macro
 */
#include <my_global.h>
#include <mysql.h>

#ifndef SRC_MODEL_INTERFACE_POST_H_
#define SRC_MODEL_INTERFACE_POST_H_

#include "../../../lib/oop/lw_new.h"
#include "base.h"

extern klass_info *post_klass;

typedef struct _post post;
struct _post {
	base parent;
};

void post_init(void);
void post_set_field_value(post *self, char *field_name, int not_form,
		char *value , field_type type);
form_data *post_get_field_value(post *self, char *field_name);

void *post_ctor(post *self);
void *post_dtor(post *self);

#endif /* SRC_MODEL_INTERFACE_POST_H_ */
