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
typedef struct _field_where field_where;
extern klass_info *base_klass;

//数据类型
enum _field_type {
	FIELD_STRING = 1, // 字符串类型
	FIELD_INT = 2, //整数类型
	FIELD_FLOAT = 3, //浮点数类型
};

enum _field_mark {
	FIELD_INSERT = 0, //正常
	FIELD_DELETE = 100, //该数据表单用于组合成删除语句的条件
	FIELD_UPDATE = 101, //该数据表单用于组合成更新语句的条件
	FIELD_SELECT = 102 //该数据表单用于组合成选择语句的条件
};

typedef enum _field_type field_type;
typedef enum _field_mark field_mark;

// 用户表单数据结构
struct _form_data {
	char *field_name;
	char *field_value;
	int field_name_length;
	int field_value_length;
	int field_type;  //该字段的类型
	int field_mark;  //该字段的标示，_field_type
};

//框架基础结构
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
//将表单数据组合成插入SQL语句
static void *compositeInsertSql(base *parent, char *field_name_sql,
		char *field_value_sql);
//将表单数据组合成删除SQL语句
static void * compositeDeteleSql(base *parent, char *field_where_sql);
//将表单数据组合成选择SQL语句
static void * compositeSelectSql(base *parent, char *field_select_sql,
		char *field_where_sql, int field_cnt, int where_cnt);
static void * compositeDeteleSql(base *parent, char *field_where_sql);
//将表单数据组合成更新SQL语句
static void * compositeUpdateSql(base *parent, char *field_update_sql,
		char *field_where_sql, int field_cnt, int where_cnt);
//为表单字段分配内存空间
void setFieldValue(base *self, char *field_name, int not_form, void *value,
		field_type type, field_mark mark);
//获取相应表单的值
form_data *getFieldValue(base *self, char *field_name);
//插入SQL语句的组合
int insertData(base *base, char *table);
//删除SQL语句的组合
int deleteData(base *base, char *table);
//更新SQL语句的组合
int updateData(base *base, char *table, int field_cnt, int where_cnt);
//选择SQL语句的组合
int selectData(base *base, char *table, int field_cnt, int where_cnt);
//转换数字到字符串
void tostring(char str[], int num);

//生成随机字符串
static int srand_called = 0;
char *random_string(char *dest);

#endif /* SRC_MODEL_INTERFACE_BASE_H_ */
