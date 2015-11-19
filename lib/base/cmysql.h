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

//存储数据库表结构
struct _tableField {
	char *field_name;
	int field_type;
	int field_length;
};

int getTableFields(const char *table, tableField ***table_field);
const void *insertData(char *table, char **field_tables, int field_cnt);
const void *updateData(char *table, void ***data, void ***where);
const void *deleteData(char *table, void ***where);
const void *selectData(char *table, void ***fields, void ***where);

//初始化数据库链接
static MYSQL *initMysql();
//执行SQL语句
const char * executeQuery(char *sql);


#endif /* LIB_BASE_CMYSQL_H_ */
