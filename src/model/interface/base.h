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
#include <my_global.h>
#include <mysql.h>
#include "../../../lib/oop/lw_new.h"
#include "../../../lib/cgic/cgic.h"

#define BASE(_object_) ((base *)_object_)

typedef struct _base base;
typedef struct _tableField tableField;
typedef struct _user_data user_data;
extern klass_info *base_klass;

// 用户业务数据
struct _user_data {
	char *field_name;
	char *field_value;
	int field_name_length;
	int field_value_length;
};

struct _base {
	klass_info *klass;
	int field_count;
	int current;
	user_data **user_datas;
	char *field_tables[5];
};

//初始化函数
void base_init(void);
//构造函数
void *base_ctor(base *self);
//析构函数
void *base_dtor(base *self);
//为表单字段分配内存
static int mallocArray(user_data ***ptr, char **field_tables, int field_cnt,
		int *field_name_length, int *field_value_length);
//释放表单的结构数据
static void * free_user_data(user_data ***userData, int cnt);
//将表单数据组合成SQL语句
static void *compositeSql(user_data ***data, char *field_name_sql,
		char *field_value_sql, int field_cnt, int field_name_length,
		int field_value_length);

void setFieldValue(base *self, char *field_name);
user_data *getFieldValue(base *self, char *field_name);

#endif /* SRC_MODEL_INTERFACE_BASE_H_ */
