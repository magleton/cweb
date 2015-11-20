#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>
#include <time.h>
#include "../../lib/cgic/cgic.h"
#include "../../lib/ctemplate/ctemplate.h"
#include "../../lib/oop/lw_new.h"
#include "../model/interface/user.h"
#include "../model/interface/base.h"

static void regUser(); //用户注册
static void login(); //用户登录
static void loginIndex();

int cgiMain() {
	cgiHeaderContentType("text/html;charset=utf-8");
	int action_len = 0;
	cgiFormStringSpaceNeeded("action", &action_len);
	char *action = (char *) malloc(sizeof(char) * (action_len + 1));
	cgiFormString("action", action, 200);
	if (strcmp(action, "reguser") == 0) { // 注册
		regUser();
	} else if (strcmp(action, "login") == 0) { //登录
		login();
	} else {
		loginIndex();
	}
	free(action);
	return 0;
}

//默认首页
static void loginIndex() {
	TMPL_write("../resource/template/login/index.html", 0, 0, 0, cgiOut,
			cgiOut);
}

static void regUser() {
	if (cgiFormSubmitClicked("btn") == cgiFormSuccess) {
		char *result = "success";
		user *user1 = lw_new(user_klass);
		char created_time[11] = { 0 };
		char updated_time[11] = { 0 };
		tostring(created_time, time(NULL));
		tostring(updated_time, time(NULL));
		user_set_field_value(user1, "username", 0, "", FIELD_STRING);
		user_set_field_value(user1, "pwd", 0, "", FIELD_STRING);
		user_set_field_value(user1, "salt", 1, "salt", FIELD_STRING);
		user_set_field_value(user1, "created_time", 1, created_time, FIELD_INT);
		user_set_field_value(user1, "updated_time", 1, updated_time, FIELD_INT);
		user_get_field_value(user1, "pwd");
		user_get_field_value(user1, "username");
		user_get_field_value(user1, "salt");
		user_get_field_value(user1, "created_time");
		user_get_field_value(user1, "updated_time");
		insertData1(BASE(user1), "user");
		//fprintf(cgiOut , "<h3 style='color:red;'>%s</h3> VALUES  %s" , field_name_sql,field_value_sql);
		lw_destory(user1);
		TMPL_varlist *varlist;
		varlist = TMPL_add_var(0, "msg", result, 0);
		TMPL_write("../resource/template/login/reguser.html", 0, 0, varlist,
				cgiOut, cgiOut);
	} else {
		TMPL_varlist *varlist;
		varlist = TMPL_add_var(0, "msg", "", 0);
		TMPL_write("../resource/template/login/reguser.html", 0, 0, varlist,
				cgiOut, cgiOut);
	}

}

//处理用户登录
static void login() {
	TMPL_varlist *varlist;
	varlist = TMPL_add_var(0, 0);
	if (cgiFormSubmitClicked("btn") == cgiFormSuccess) { //处理提交
		char username[20];
		char pwd[20];
		int content_length;
		cgiFormStringSpaceNeeded("content", &content_length);
		char *content = (char *) malloc(sizeof(char) * (content_length + 1));
		cgiFormString("username", username, 20);
		cgiFormString("pwd", pwd, 20);
		cgiFormString("content", content, content_length);
		char insertSql[1024] = { 0 };
		sprintf(insertSql,
				"INSERT INTO `post` (`keywords`, `content`) VALUES ('%s', '%s');",
				username, content);
		//executeQuery(insertSql);
		varlist = TMPL_add_var(varlist, "msg", "已经提交了哦！！", 0);
		varlist = TMPL_add_var(varlist, "username", username, 0);
		varlist = TMPL_add_var(varlist, "pwd", pwd, 0);
		free(content);
	} else {
		varlist = TMPL_add_var(varlist, "msg", "初始化", 0);
	}
	TMPL_write("../resource/template/login/login.html", 0, 0, 0, cgiOut,
			cgiOut);
	TMPL_free_varlist(varlist);
}
