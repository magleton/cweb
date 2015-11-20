/*
 * cmysql.h
 *
 *  Created on: 2015年11月19日
 *      Author: macro
 */

#ifndef LIB_BASE_CMYSQL_H_
#define LIB_BASE_CMYSQL_H_
#include <stdio.h>
#include <stdlib.h>
#include <my_global.h>
#include <mysql.h>
#include "../oop/lw_new.h"

typedef struct _tableField tableField;
//存储数据库表结构
struct _tableField {
	char *field_name;
	int field_type;
	int field_length;
};

int getTableFields(const char *table, tableField ***table_field);

//初始化数据库链接
MYSQL *initMysql();
//执行SQL语句
const char * executeQuery(char *sql);

#endif /* LIB_BASE_CMYSQL_H_ */
