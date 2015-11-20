#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../interface/post.h"
#include "../interface/base.h"
#include "../../../lib/oop/lw_new.h"
#include "../../../lib/cgic/cgic.h"

klass_info local_post_klass = { post_init };
klass_info *post_klass = &local_post_klass;

void post_init(void) {
	if (post_klass->init) {
		base_init();
		memcpy(post_klass, base_klass, sizeof(klass_info));
		post_klass->super = base_klass;
		post_klass->class_name = "post_klass";
		post_klass->size = sizeof(post);
		post_klass->ctor = post_ctor;
		post_klass->dtor = post_dtor;
	}
}

void post_set_field_value(post *self, char *field_name, int not_form,
		char *value, field_type type) {
	setFieldValue(BASE(self), field_name, not_form, value, type);
}

form_data *post_get_field_value(post *self, char *field_name) {
	form_data *current_post_data = getFieldValue((BASE(self)), field_name);
	if (current_post_data) {
#ifdef SHOW_FIELD_KEY_VALUE
		fprintf(cgiOut, "%s = %s<br/>", current_post_data->field_name,
				current_post_data->field_value);
#endif
		return current_post_data;
	}
	return NULL;
}

void *post_ctor(post *self) {
	((void_fun) (klass_of(self)->super->ctor))(self);
	(BASE(self))->field_count = 5;
	(BASE(self))->current = 0;
	(BASE(self))->form_datas = (form_data **) malloc(sizeof(form_data *) * 5);
	(BASE(self))->field_tables = (char **) malloc(sizeof(char *) * 5);
	return self;
}

void *post_dtor(post *self) {
	(((void_fun) (klass_of(self)->super->dtor))(self));
	return self;
}

