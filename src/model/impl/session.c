/*
 * session.c
 *
 *  Created on: 2015年11月26日
 *  Author: macro
 * 实现 session 支持
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "../interface/session.h"
#include "../../../lib/cgic/cgic.h"

SESSION_DATA* g_session_data = NULL;
const char* g_session_filename_prefix = "sess_";

/**
 * 为当前访问者启用 session
 *
 * @param const char* datadir 用于保存 session 数据文件的目录
 *
 * @return int 成功返回0，失败返回非0值
 */
int session_start(const char* datadir) {
	char cookie_session_id[34];
	int result;

	if (g_session_data != NULL) {
		fprintf(cgiOut, "Warning: session_start() calls again.<br />\n");
		return -1;
	}

	// 初始化 session_data
	sess_init(datadir);

	// 根据 cookie 状态确定是创建新 session 还是载入现有的 session
	result = cgiCookieString("SESSION_ID", cookie_session_id, 33);
#ifdef DEBUG
	fprintf(cgiOut, "cgiCookieString() result: %u\n", result);
	fprintf(cgiOut, "cookie_session_id: %s\n", cookie_session_id);
#endif

	if (result != cgiFormSuccess) {
		// 没有找到有效的 cookie session_id，创建一个新 session
		result = sess_create();
	} else {
		result = sess_load(cookie_session_id, datadir);
	}
	return result;
}

/**
 * 关闭 session 并将 session 数据写入临时文件并关闭 session
 *
 * @return int 成功返回0，失败返回非0值
 */
int session_write_close() {
	FILE* fp = NULL;
	size_t i = 0;
	char* buffer = NULL;

	if (g_session_data == NULL) {
		fprintf(cgiOut, "Warning: You must make your calls to session_close()"
				" AFTER invoking session_start().<br />\n");
		return -1;
	}

#ifdef DEBUG
	fprintf(cgiOut, "session_write_close() <-- BEGIN\n");
#endif

	fp = fopen(g_session_data->session_filename, "w");
	if (fp == NULL) {
		fprintf(cgiOut, "Warning: Can't create session file: %s.<br />\n",
				g_session_data->session_filename);
		return -1;
	}

	// 开始写入 session 数据
	for (i = 0; i < g_session_data->max_index; i++) {
		if (g_session_data->items[i].name == NULL) {
			continue;
		}

		buffer = sess_serialize(g_session_data->items[i].name);
		fputs(buffer, fp);
		fputs("\n", fp);
		free(buffer);
		buffer = NULL;

		buffer = sess_serialize(g_session_data->items[i].string);
		fputs(buffer, fp);
		fputs("\n", fp);
		free(buffer);
		buffer = NULL;
	}
	fclose(fp);

#ifdef DEBUG
	session_dump(cgiOut);
	fprintf(cgiOut, "session_write_close() END -->\n");
#endif

	// 释放 session 占用的资源
	sess_free_resource();

	return 0;
}

/**
 * 销毁当前访问者的 session
 *
 * @return int 成功返回0，失败返回非0值
 */
int session_destroy() {
	if (g_session_data == NULL) {
		fprintf(cgiOut, "Warning: You must make your calls to session_destroy()"
				" AFTER invoking session_start().<br />\n");
		return -1;
	}

#ifdef DEBUG
	fprintf(cgiOut, "session_destroy()\n");
	session_dump(cgiOut);
#endif

	// 删除 session 数据文件
	remove(g_session_data->session_filename);
	// 释放 session 占用的资源
	sess_free_resource();

	return 0;
}

/**
 * 设置要保存在 session 中的字符串
 *
 * @param const char* name 字符串的名字
 * @param const char* string 字符串
 *
 * @return int 成功返回0，失败返回非0值
 */
int session_set(const char* name, const char* string) {
	int i;
	SESSION_ITEM_LIST tmp_list = NULL;
	size_t buffer_size;

#ifdef DEBUG
	fprintf(cgiOut, "session_set(), session file:%s\n", g_session_data->session_filename);
#endif

	for (i = 0; i < g_session_data->max_index; i++) {
		if (g_session_data->items[i].name == NULL) {
			// 找到空闲的项目
			g_session_data->items[i].name = (char*) malloc(
					(strlen(name) + 1) * sizeof(char));
			strcpy(g_session_data->items[i].name, name);
			g_session_data->items[i].string = (char*) malloc(
					(strlen(string) + 1) * sizeof(char));
			strcpy(g_session_data->items[i].string, string);
			g_session_data->count++;
			return 0;
		}
	}

	// 需要扩充容量
	i = g_session_data->max_index + (g_session_data->max_index / 2) + 2;
	buffer_size = i * sizeof(SESSION_ITEM);
	tmp_list = (SESSION_ITEM*) malloc(buffer_size);
	memset(tmp_list, 0, buffer_size);
	memcpy(tmp_list, g_session_data->items,
			g_session_data->max_index * sizeof(SESSION_ITEM));
	free(g_session_data->items);
	g_session_data->items = tmp_list;
	g_session_data->max_index = i;

	return session_set(name, string);
}

/**
 * 取出保存在 session 中的字符串
 *
 * @param const char* name 字符串的名字
 *
 * @return const char* 保存的字符串，如果没找到则返回 NULL
 */
const char* session_get(const char* name) {
	size_t i;

	if (g_session_data == NULL) {
		fprintf(cgiOut, "Warning: You must make your calls to session_get()"
				" AFTER invoking session_start().<br />\n");
		return NULL;
	}

#ifdef DEBUG
	fprintf(cgiOut, "session_get(), session file:%s\n", g_session_data->session_filename);
#endif

	for (i = 0; i < g_session_data->max_index; i++) {
		if (g_session_data->items[i].name == NULL) {
			continue;
		}
		if (strcmp(g_session_data->items[i].name, name) == 0) {
			return g_session_data->items[i].string;
		}
	}

	return NULL;
}

/**
 * 从 session 中删除指定的字符串
 *
 * @param const char* name 字符串的名字
 *
 * @return int 成功返回0，失败返回非0值
 */
int session_unset(const char* name) {
	size_t i;

	if (g_session_data == NULL) {
		fprintf(cgiOut, "Warning: You must make your calls to session_unset()"
				" AFTER invoking session_start().<br />\n");
		return -1;
	}

#ifdef DEBUG
	fprintf(cgiOut, "session_unset(), session file:%s\n", g_session_data->session_filename);
#endif

	for (i = 0; i < g_session_data->max_index; i++) {
		if (strcmp(g_session_data->items[i].name, name) == 0) {
			// 找到匹配的项目
			free(g_session_data->items[i].name);
			free(g_session_data->items[i].string);
			g_session_data->items[i].name = NULL;
			g_session_data->items[i].string = NULL;
			g_session_data->count--;
			return 0;
		}
	}

	return -1;
}

/**
 * 从 session 中删除所有数据
 *
 * @return int 成功返回0，失败返回非0值
 */
int session_unset_all() {
	size_t i;

	if (g_session_data == NULL) {
		fprintf(cgiOut, "Warning: You must make your calls to session_destroy()"
				" AFTER invoking session_start().<br />\n");
		return -1;
	}

	for (i = 0; i < g_session_data->max_index; i++) {
		if (g_session_data->items[i].name == NULL) {
			continue;
		}
		free(g_session_data->items[i].name);
		free(g_session_data->items[i].string);
		g_session_data->items[i].name = NULL;
		g_session_data->items[i].string = NULL;
	}
	g_session_data->count = 0;

	return 0;
}

/**
 * 初始化 session_data
 *
 * @param const char* datadir
 */
void sess_init(const char* datadir) {
	if (g_session_data != NULL) {
		return;
	}
	// 为 session 数据分配内存
	g_session_data = (SESSION_DATA*) malloc(sizeof(SESSION_DATA));
	memset(g_session_data, 0, sizeof(SESSION_DATA));
	g_session_data->session_datadir = (char*) malloc(
			(strlen(datadir) + 2) * sizeof(char));
	strcpy(g_session_data->session_datadir, datadir);
	if (datadir[strlen(datadir) - 1] != '/') {
		strcat(g_session_data->session_datadir, "/");
	}
}

/**
 * 创建一个新 session
 *
 * @return int 成功返回0，失败返回非0值
 */
int sess_create() {
	const int init_index = 10;
	int i;

	if (g_session_data == NULL) {
		return -1;
	}

#ifdef DEBUG
	fprintf(cgiOut, "sess_create()\n");
#endif

	// 构造 session id
	g_session_data->session_id = sess_make_session_id();
	g_session_data->session_filename = sess_get_filename(
			g_session_data->session_datadir, g_session_data->session_id);

	// 初始化 session item 列表的存储空间
	g_session_data->items = (SESSION_ITEM*) malloc(
			init_index * sizeof(SESSION_ITEM));
	for (i = 0; i < init_index; i++) {
		g_session_data->items[i].name = NULL;
		g_session_data->items[i].string = NULL;
	}
	g_session_data->max_index = init_index;

	// 写入 cookie
	cgiHeaderCookieSetString("SESSION_ID", g_session_data->session_id, 900, "/",
			cgiServerName);

	return 0;
}

/**
 * 载入现有的 session
 *
 * @param const char* session_id 要读取的 session_id
 * @param const char* datadir
 *
 * @return int 成功返回0，失败返回非0值
 */
int sess_load(const char* session_id, const char* datadir) {
	FILE* fp = NULL;
	char* buffer = NULL;
	char* tmp = NULL;
	SESSION_ITEM_LIST tmp_list =
	NULL;
	size_t buffer_length = 2048;
	int index = 0;

	if (g_session_data == NULL) {
		return -1;
	}

#ifdef DEBUG
	fprintf(cgiOut, "sess_load() BEGIN -->\n");
#endif

	// 构造 session_data
	g_session_data->session_id = (char*) malloc(
			(strlen(session_id) + 1) * sizeof(char));
	strcpy(g_session_data->session_id, session_id);
	g_session_data->session_filename = sess_get_filename(
			g_session_data->session_datadir, g_session_data->session_id);

#ifdef DEBUG
	fprintf(cgiOut, "sess_load(), session file:%s\n", g_session_data->session_filename);
#endif

	fp = fopen(g_session_data->session_filename, "r");
	if (fp == NULL) {
		// 无效的 session_filename
		sess_free_resource();
		sess_init(datadir);
		return sess_create();
	}

	buffer = (char*) malloc(buffer_length * sizeof(char));
	tmp = buffer;
	while (!feof(fp)) {
		if (fgets(tmp, buffer_length, fp) == NULL) {
			break;
		}

		/*
		 if (buffer[strlen(buffer) + 1] != '\n' && !feof(fp)) {
		 // 缓冲区太小，无法容纳该行字符串
		 buffer_length += buffer_length;
		 tmp = (char*)malloc(buffer_length * sizeof(char));
		 strcpy(tmp, buffer);
		 free(buffer);
		 buffer = tmp;
		 tmp = buffer += strlen(buffer);
		 continue;
		 }
		 */

		// 如果 count >= max_index，则需要增加列表容量
		if (g_session_data->count >= g_session_data->max_index) {
			g_session_data->max_index += 10;
#ifdef DEBUG
			fprintf(cgiOut, "* items count: %ld, max_index grow up to: %ld\n",
					g_session_data->count,
					g_session_data->max_index);
#endif
			tmp_list = (SESSION_ITEM_LIST) malloc(
					g_session_data->max_index * sizeof(SESSION_ITEM));
			memset(tmp_list, 0,
					g_session_data->max_index * sizeof(SESSION_ITEM));
			if (g_session_data->items != NULL) {
				memcpy(tmp_list, g_session_data->items,
						g_session_data->count * sizeof(SESSION_ITEM));
				free(g_session_data->items);
			}
			g_session_data->items = tmp_list;
			tmp_list = NULL;
		}

		// 添加一个新项目
		if ((index % 2) == 0) {
			tmp = g_session_data->items[g_session_data->count].name;
#ifdef DEBUG
			fprintf(cgiOut, "read name:");
#endif
		} else {
			tmp = g_session_data->items[g_session_data->count].string;
#ifdef DEBUG
			fprintf(cgiOut, "read string:");
#endif
		}
		if (buffer[strlen(buffer) - 1] == '\n') {
			buffer[strlen(buffer) - 1] = '\0';
		}
		tmp = sess_unserialize(buffer);
#ifdef DEBUG
		fprintf(cgiOut, "%s\n", tmp);
#endif
		if ((index % 2) == 0) {
			g_session_data->items[g_session_data->count].name = tmp;
		} else {
			g_session_data->items[g_session_data->count].string = tmp;
			g_session_data->count++;
		}

		// 增加索引，准备读取下一行
		tmp = buffer;
		index++;
	}
	free(buffer);
	fclose(fp);

	if ((index % 2) != 0) {
		free(g_session_data->items[g_session_data->count - 1].name);
		g_session_data->items[g_session_data->count - 1].name =
		NULL;
	}

#ifdef DEBUG
	session_dump(cgiOut);
#endif

#ifdef DEBUG
	fprintf(cgiOut, "sess_load() END -->\n");
#endif

	return 0;
}

/**
 * 根据 session_id 生成文件名
 *
 * @param const char* datadir
 * @param const char* session_id
 *
 * @return char* 文件名
 */
char* sess_get_filename(const char* datadir, const char* session_id) {
	size_t buffer_size;
	char* session_filename;
	char* result;

	// 再次 md5 是为了避免浏览器送来的畸形 cookie 导致的安全隐患
	session_filename = sess_md5_calc(session_id);
	// 生成 session 文件的完整路径
	buffer_size = (strlen(datadir) + strlen(g_session_filename_prefix)
			+ strlen(session_filename)) * sizeof(char);

	result = (char*) malloc(buffer_size);
	memset(result, 0, buffer_size);
	strcpy(result, datadir);
	strcat(result, g_session_filename_prefix);
	strcat(result, session_filename);
	free(session_filename);
	return result;
}

/**
 * 释放 session 资源
 */
void sess_free_resource() {
	size_t i;

	if (g_session_data == NULL) {
		return;
	}

	// 销毁 session 内存映像
	for (i = 0; i < g_session_data->max_index; i++) {
		if (g_session_data->items[i].name == NULL) {
			continue;
		}
		free(g_session_data->items[i].name);
		free(g_session_data->items[i].string);
	}

	free(g_session_data->items);
	free(g_session_data->session_filename);
	free(g_session_data->session_id);
	free(g_session_data->session_datadir);
	free(g_session_data);
	g_session_data =
	NULL;
}

/**
 * 根据当前访问者的 IP 地址和浏览器信息等参数生成 session id
 *
 * @return char* 保存新 session_id 的缓冲区指针
 */
char* sess_make_session_id() {
	char* buffer = NULL;
	char* session_id = NULL;
	char* remote_port = NULL;
	size_t length = 0;

	//remote_port = getenv("REMOTE_PORT");
	length = strlen(cgiRemoteAddr);
	//length += strlen(cgiUserAgent);
	length += strlen(cgiRemoteHost);
	if (remote_port != NULL) {
		length += strlen(remote_port);
	}

	buffer = (char*) malloc((length + 1) * sizeof(char));
	memset(buffer, 0, (length + 1) * sizeof(char));

	strcpy(buffer, cgiRemoteAddr);
//	strcat(buffer, cgiUserAgent);
	strcat(buffer, cgiRemoteHost);
	if (remote_port != NULL) {
		strcat(buffer, remote_port);
	}

	session_id = sess_md5_calc(buffer);
	free(buffer);
#ifdef DEBUG
	fprintf(cgiOut, "<div style='color:yellow;'>%s</div>", remote_port);
#endif
	return session_id;
}

/**
 * 根据提供的字符串计算 md5 hash
 *
 * @param const char* string 要计算 hash 字符串
 *
 * @return char* 保存 md5 hash 的缓冲区指针
 */
char* sess_md5_calc(const char* string) {
	MD5_CTX md5ctx;
	char* buffer = (char*) malloc(33 * sizeof(char));
	memset(buffer, 0, 33 * sizeof(char));
	strncpy(buffer, string, sizeof(char) * 33);
	MD5Init(&md5ctx);
	//MD5Data(string, strlen(string), buffer);
	MD5Update(&md5ctx, buffer, strlen(buffer));
	//MD5Final(&md5ctx, decrypt);
#ifdef DEBUG1
	fprintf(cgiOut, "加密前:%s\n加密后16位:", buffer);
#endif
	return buffer;
}

/**
 * 序列化字符串
 *
 * @param const char* string 要序列化的字符串
 *
 * @return char* 序列化以后的字符串
 */
char* sess_serialize(const char* string) {
	size_t buffer_size = 0;
	char* buffer = NULL;
	char* pos = (char *) string;
	char ch;

	// 首先确定序列化以后的字符串长度并分配内存
	while (*pos) {
		if (*pos == '\n') {
			buffer_size += sizeof(char);
		}
		buffer_size += sizeof(char);
		pos++;
	}
	buffer_size += sizeof(char);
	buffer = (char*) malloc(buffer_size);

	// 将所有的 \n 替换为 \\n
	pos = buffer;
	while ((ch = *string) != '\0') {
		if (ch == '\n') {
			*pos = '\\';
			pos++;
			ch = 'n';
		}

		*pos = ch;
		pos++;
		string++;
	}

	*pos = '\0';
	return buffer;
}

/**
 * 反序列化字符串
 *
 * @param const char* string 要反序列化的字符串
 *
 * @return char* 反序列化以后的字符串
 */
char* sess_unserialize(const char* string) {
	size_t buffer_size = (strlen(string) * 2) * sizeof(char);
	char* buffer = (char*) malloc(buffer_size);
	char* src = (char *) string;
	char* dest = buffer;
	memset(buffer, 0, buffer_size);

	while (*src) {
		if (*src == '\\' && *(src + 1) == 'n') {
			*dest = '\n';
			src++;
		} else {
			*dest = *src;
		}
		src++;
		dest++;
	}

	dest = (char*) malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(dest, buffer);
	free(buffer);
	return dest;
}

/**
 * 输出 session 的完整内容，用于调试目的
 */
void session_dump(FILE* fp) {
	int i;
	if (g_session_data == NULL) {
		return;
	}
	fprintf(fp, "---------- SESSION dump ----------\n");
	fprintf(fp, "session_datadir: %s\n", g_session_data->session_datadir);
	fprintf(fp, "session_filename: %s\n", g_session_data->session_filename);
	fprintf(fp, "session_id: %s\n", g_session_data->session_id);
	fprintf(fp, "count: %ld\n", g_session_data->count);
	fprintf(fp, "max_index: %ld\n", g_session_data->max_index);

	for (i = 0; i < g_session_data->max_index; i++) {
		if (g_session_data->items[i].name == NULL) {
			continue;
		}
		fprintf(fp, "  Item[%u].name = %s\n", i, g_session_data->items[i].name);
		fprintf(fp, "  Item[%u].string = %s\n", i,
				g_session_data->items[i].string);
	}
	fprintf(fp, "---------- Dump end ----------\n");
}
