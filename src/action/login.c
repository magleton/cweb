#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>
#include <time.h>
#include "../../lib/cgic/cgic.h"
#include "../../lib/ctemplate/ctemplate.h"
#include "../../lib/oop/lw_new.h"
#include "../../lib/base/cmysql.h"
#include "../model/interface/user.h"
#include "../model/interface/post.h"
#include "../model/interface/base.h"
#include "../model/interface/upload.h"
#include "../model/interface/session.h"

static void regUser(); //用户注册
static void login(); //用户登录
static void loginIndex();

int cgiMain() {
	cgiHeaderContentType("text/html;charset=utf-8");
	int action_len = 0;
	session_start("/tmp/");
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
		//post *post1 = lw_new(post_klass);
		char created_time[11] = { 0 };
		char updated_time[11] = { 0 };
		char tfloat[20] = { 0 };
		char origin_salt[20] = { 0 };
		char *salt = random_string(origin_salt);
		int user_id = 0;
		char user_id_str[11];
		//tostring(created_time, time(NULL));
		tostring(updated_time, time(NULL));
		tableField **table_field;
		int i, field_cnt = 0;
		field_cnt = getTableFields("user", &table_field); //获取表的信息
		for (i = 0; i < field_cnt; ++i) {
			if (strcmp(table_field[i]->field_name, "id")) { //排除掉ID字段
				fprintf(cgiOut,
						"field_cnt= %d Field =%s Type=%d Length=%d<br/>",
						field_cnt, table_field[i]->field_name,
						table_field[i]->field_type,
						table_field[i]->field_length);
			}
		}
		char fileName[200] = { 0 };
		char errorMsg[200] = { 0 };
		int hint = -1;
		hint = uploadFile(fileName, errorMsg);
		if (hint != 0) {
			fprintf(cgiOut,
					"<div style='color:red'>fileName = %s Failed to upload , error_msg = %s , hint=%d</div>",
					fileName, errorMsg, hint);
		} else {
			fprintf(cgiOut,
					"<div style='color:red'>%s has been uploaded!</div>",
					fileName);
		}
		snprintf(created_time, sizeof(created_time), "%ld", time(NULL));
		/*user_set_field_value(user1, "username", 0, "", FIELD_STRING,
		 FIELD_INSERT);*/
		user_set_field_value(user1, "pwd", 0, "", FIELD_STRING, FIELD_INSERT);
		user_set_field_value(user1, "id", 1, "1", FIELD_STRING, FIELD_SELECT);
		user_set_field_value(user1, "created_time", 1, created_time, FIELD_INT,
		 FIELD_INSERT);
		user_set_field_value(user1, "updated_time", 1, updated_time, FIELD_INT,
		FIELD_INSERT);
		insertData(BASE(user1), "user");
		/*user_get_field_value(user1, "pwd");
		 user_get_field_value(user1, "username");
		 user_get_field_value(user1, "salt");
		 user_get_field_value(user1, "created_time");
		 user_get_field_value(user1, "updated_time");*/
		//user_id = insertData(BASE(user1), "user");  //插入操作
		//deleteData(BASE(user1), "user"); //删除操作
		//selectData(BASE(user1) , "user" , 1 , 1); //选择操作
		//updateData(BASE(user1) , "user" , 1 ,1); //选择操作
		//tostring(user_id_str, user_id);
		/*post_set_field_value(post1, "user_id", 1, user_id_str, FIELD_STRING);
		 post_set_field_value(post1, "title", 1, "你好", FIELD_STRING);
		 post_set_field_value(post1, "content", 1, "这个是内容", FIELD_STRING);
		 post_set_field_value(post1, "created_time", 1, created_time, FIELD_INT);
		 post_set_field_value(post1, "updated_time", 1, updated_time, FIELD_INT);
		 insertData(BASE(post1), "post");
		 post_get_field_value(post1, "user_id");
		 post_get_field_value(post1, "title");
		 post_get_field_value(post1, "content");
		 post_get_field_value(post1, "created_time");
		 post_get_field_value(post1, "updated_time");*/
		//fprintf(cgiOut , "<h3 style='color:red;'>%s</h3> VALUES  %s" , field_name_sql,field_value_sql);
		//lw_destory(user1);
		lw_destory(user1);
		//lw_destory(post1);
		//fprintf(cgiOut , "<h3 style='color:red;'>%s</h3>" , salt);

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
		// 登录验证通过，设置 session 数据后重定向浏览器
					session_set("USERNAME", "aaaaaaaaa");
					session_write_close();
		char insertSql[1024] = { 0 };
		/*sprintf(insertSql,
				"INSERT INTO `post` (`keywords`, `content`) VALUES ('%s', '%s');",
				username, content);*/
		//executeQuery(insertSql);
		/*varlist = TMPL_add_var(varlist, "msg", "已经提交了哦！！", 0);
		varlist = TMPL_add_var(varlist, "username", username, 0);
		varlist = TMPL_add_var(varlist, "pwd", pwd, 0);*/
		free(content);
	} else {
		varlist = TMPL_add_var(varlist, "msg", "初始化", 0);
	}
	TMPL_write("../resource/template/login/login.html", 0, 0, 0, cgiOut,
			cgiOut);
	TMPL_free_varlist(varlist);
}
