/*
 * ooptest.c
 *
 *  Created on: 2015年11月18日
 *      Author: macro
 */

#include <stdio.h>
#include <string.h>
#include "../../lib/cgic/cgic.h"
#include "../../lib/ctemplate/ctemplate.h"
#include "../../lib/base/base.h"
#include "../../lib/oop/lw_new.h"
#include "../model/interface/user.h"
int cgiMain() {
	cgiHeaderContentType("text/html;charset=gbk");
	user *user1 = lw_new(user_klass);
	user_set_username(user1 , "macro chen");
	fprintf(cgiOut , "%s\n" , user_get_username(user1));
	lw_destory(user1);
	return 0;
}
