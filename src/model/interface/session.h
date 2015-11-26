/*
 * session.h
 *
 *  Created on: 2015年11月26日
 *  Author: macro
 *  实现 session 支持
 */

#ifndef SRC_MODEL_INTERFACE_SESSION_H_
#define SRC_MODEL_INTERFACE_SESSION_H_

#include <stdio.h>
#include <string.h>
#include "md5.h"

typedef struct {
	char* name;
	char* string;
} SESSION_ITEM, *SESSION_ITEM_LIST;

typedef struct {
	char* session_datadir;
	char* session_filename;
	char* session_id;
	size_t count;
	size_t max_index;
	SESSION_ITEM_LIST items;
} SESSION_DATA;

extern SESSION_DATA* g_session_data;
extern const char* g_session_filename_prefix;

/**
 * 以下函数由使用者调用
 */
int session_start(const char* datadir);
int session_write_close();
int session_destroy();
int session_set(const char* name, const char* string);
const char* session_get(const char* name);
int session_unset(const char* name);
int session_unset_all();
void session_dump(FILE* fp);

/**
 * 以下函数为内部使用，不应由使用者调用
 */
void sess_init(const char* datadir);
int sess_create();
int sess_load(const char* session_id, const char* datadir);
char* sess_get_filename(const char* datadir, const char* session_id);
void sess_free_resource();
char* sess_make_session_id();
char* sess_md5_calc(const char* string);
char* sess_serialize(const char* string);
char* sess_unserialize(const char* string);

#endif /* SRC_MODEL_INTERFACE_SESSION_H_ */

