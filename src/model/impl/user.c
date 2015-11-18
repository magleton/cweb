/*
 * user.c
 *
 *  Created on: 2015年11月18日
 *      Author: macro
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../interface/user.h"
#include "../../../lib/base/base.h"
#include "../../../lib/oop/lw_new.h"

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

char *user_get_username(user *self) {
	return self->username;
}

void user_set_username(user *self, char *username) {
	strncpy(self->username, username, sizeof(self));
}

char *user_get_pwd(user *self) {
	return self->pwd;
}
void user_set_pwd(user *self, char *pwd) {
	strncpy(self->pwd, pwd, sizeof(self->pwd));
}

char *user_get_salt(user *self) {
	return self->salt;
}

void user_set_salt(user *self, char *salt) {
	strncpy(self->salt, salt, sizeof(self->salt));
}

void *user_ctor(user *self) {
	((void_fun) (klass_of(self)->super->ctor))(self);
	self->created_time = time(NULL);
	self->updated_time = time(NULL);
	return self;
}

void *user_dtor(user *self) {
	((void_fun) (klass_of(self)->super->dtor))(self);
	return self;
}
