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
const char * executeQuery(char *sql);
static MYSQL *initMysql();
int getModelField();

//存储数据库表结构
typedef struct _tableField {
	char *field_name;
	int field_type;
	int field_length;
} tableField;

#endif /* SRC_MODEL_INTERFACE_POST_H_ */
