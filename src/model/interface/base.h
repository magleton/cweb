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
typedef struct _form_data form_data;
extern klass_info *base_klass;

typedef enum _field_type field_type;
//数据类型
enum _field_type {
	FIELD_STRING = 1, FIELD_INT = 2 , FIELD_FLOAT = 3
};

// 用户表单数据结构
struct _form_data {
	char *field_name;
	char *field_value;
	int field_name_length;
	int field_value_length;
	int field_type;
};

struct _base {
	klass_info *klass;
	int field_count;
	int current;
	form_data **form_datas;
	char **field_tables;
};

//初始化函数
void base_init(void);
//构造函数
void *base_ctor(base *self);
//析构函数
void *base_dtor(base *self);
//将表单数据组合成SQL语句
static void *compositeSql(base *parent, char *field_name_sql,
		char *field_value_sql);
//为表单字段分配内存空间
void setFieldValue(base *self, char *field_name, int not_form, char *value,
		field_type type);
//获取相应表单的值
form_data *getFieldValue(base *self, char *field_name);
//插入SQL的组合
const void *insertData1(base *base, char *table);
//转换数字到字符串
void tostring(char str[], int num);

#endif /* SRC_MODEL_INTERFACE_BASE_H_ */
