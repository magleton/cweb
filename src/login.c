#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <my_global.h>
#include <mysql.h>
#include "../lib/cgic/cgic.h"
#include "../lib/ctemplate/ctemplate.h"

int cgiMain() {
	cgiHeaderContentType("text/html;charset=utf-8");
	TMPL_varlist *varlist;
	varlist = TMPL_add_var(0,"asda","adsads", 0);
	if (cgiFormSubmitClicked("btn") == cgiFormSuccess) { //处理提交
		char username[20];
		char pwd[20];
		int content_length;
		my_ulonglong last_ins_id;
		cgiFormStringSpaceNeeded("content", &content_length);
		char *content = (char *) malloc(sizeof(char) * content_length);
		MYSQL *con = mysql_init(0);
		if (con == NULL) {
			varlist = TMPL_add_var(varlist, "mysql_error", mysql_error(con), 0);
			goto exit;
		}
		if (mysql_real_connect(con, "localhost", "root", "root", "test", 0,
		NULL, 0) == NULL) {
			varlist = TMPL_add_var(varlist, "mysql_error", mysql_error(con), 0);
			mysql_close(con);
			goto exit;
		}

		if (mysql_query(con, "set names utf8")) {
			varlist = TMPL_add_var(varlist, "mysql_error", mysql_error(con), 0);
			mysql_close(con);
			goto exit;
		}

		cgiFormString("username", username, 20);
		cgiFormString("pwd", pwd, 20);
		//cgiFormString("content", content , content_length);

		if (mysql_query(con,
				"INSERT INTO `post` (`keywords`, `content`) VALUES ('高粱酒高粱酒高粱酒高粱酒', 'bbbbb');")) {
			varlist = TMPL_add_var(varlist, "mysql_error", mysql_error(con), 0);
			mysql_close(con);
			goto exit;
		}
		char str[25];
		sprintf(str, " %d" , content_length);
		varlist = TMPL_add_var(varlist, "msg", "已经提交了哦！！", 0);
		varlist = TMPL_add_var(varlist, "username", username, 0);
		varlist = TMPL_add_var(varlist, "pwd", pwd, 0);
		varlist = TMPL_add_var(varlist, "content_length",str, 0);
		free(content);
	} else {
		varlist = TMPL_add_var(varlist, "msg", "初始化", 0);
	}
	exit: TMPL_write("../resource/template/login/login.html", 0, 0, varlist,
			cgiOut, cgiOut);
	TMPL_free_varlist(varlist);
	return 0;
}
