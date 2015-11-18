/*
 * base.h
 *
 *  Created on: 2015年11月18日
 *      Author: macro
 */

#ifndef SRC_MODEL_INTERFACE_BASE_H_
#define SRC_MODEL_INTERFACE_BASE_H_
#include <stdio.h>
#include <stdlib.h>
#include "../lib/oop/lw_new.h"

typedef struct _base base;
typedef struct _tableField tableField;
extern klass_info *base_klass;

struct _base {
	klass_info *klass;
};

typedef struct _user_data user_data;

// 用户业务数据
struct _user_data {
	char *field_name;
	char *field_value;
	int field_length;
};

//存储数据库表结构
struct _tableField {
	char *field_name;
	int field_type;
	int field_length;
} tableField;

void *getTableFields(char *table, void ***table_field);
void *insertData(char *table, void ***data, char **field_tables, int field_cnt);
void *updateData(char *table, void ***data, void ***where);
void *deleteData(char *table, void ***where);
void *selectData(char *table, void ***fields, void ***where);

//初始化数据库链接
static MYSQL *initMysql();
//将表单数据组合成SQL语句
static void *compositeSql(user_data ***data, char *field_name_sql,
		char *field_value_sql, int field_cnt);
//执行SQL语句
const char * executeQuery(char *sql);
//为表单字段分配内存
static int mallocArray(user_data ***ptr, char **field_tables, int field_cnt,
		int *field_name_length, int *field_value_length);
//释放表单的结构数据
static void * free_user_data(user_data ***userData, int cnt);

#endif /* SRC_MODEL_INTERFACE_BASE_H_ */
