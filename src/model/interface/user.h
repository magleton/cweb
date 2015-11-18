/*
 * user.h
 *
 *  Created on: 2015年11月18日
 *      Author: macro
 */

#ifndef SRC_MODEL_INTERFACE_USER_H_
#define SRC_MODEL_INTERFACE_USER_H_

#include "../../../lib/oop/lw_new.h"
#include "../../../lib/base/base.h"

extern klass_info *user_klass;

typedef struct _user user;
struct _user{
	klass_info *klass;
	char username[200];
	char pwd[200];
	char salt[200];
	int created_time;
	int updated_time;
};

void user_init(void);
char *user_get_username(user *user);
void user_set_username(user *user , char *username);
char *user_get_pwd(user *user);
void user_set_pwd(user *user , char *pwd);
char *user_get_salt(user *user);
void user_set_salt(user *user , char *salt);


void *user_ctor(user *user);
void *user_dtor(user *user);


#endif /* SRC_MODEL_INTERFACE_USER_H_ */
