/*
 * sessiontest.c
 *
 *  Created on: 2015年11月26日
 *  Author: macro
 *  测试session
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../lib/cgic/cgic.h"
#include "../model/interface/session.h"

/**
 * 保存要替换的标记及其替换值
 */
typedef struct {
	const char* tag; // 要替换的标记
	const char* value; // 替换值
} REPLACEABLE_TAG, *REPLACEABLE_TAG_LIST;

// 模板文件名
const char* g_login_template = "../../resource/template/login/login.html";
const char* g_control_template = "../../resource/template/login/index.html";

// 保存 session 的目录
const char* g_session_datadir = "/tmp/";

// 登录用户名和密码
const char* g_login_username = "root";
const char* g_login_password = "123456";

// 函数定义
char* read_template(const char* template_filename);
int parse_template(char** template_content, REPLACEABLE_TAG_LIST tag_list,
		int tag_count);
int login_process();
int control_process();
void control_led(unsigned char number);

int cgiMain() {
	int result;
#ifdef DEBUG
	fprintf(stderr, "========== LED.CGI Start ===========.\n");
#endif
	// 初始化 session
	session_start(g_session_datadir);

	do {
		if (session_get("USERNAME") != NULL) {
			if (strcmp(session_get("USERNAME"), g_login_username) == 0) {
				result = control_process();
				break;
			}
		}
		result = login_process();
	} while (0);

#ifdef DEBUG
	fprintf(cgiOut, "========== LED.CGI End ===========.\n");
#endif

	return result;
}

/**
 * 显示登录页面以及处理用户登录
 */
int login_process() {
	char* username = NULL;
	char* password = NULL;
	char* buffer = NULL;
	int field_buffer_size = 0;
	REPLACEABLE_TAG_LIST tag_list = (REPLACEABLE_TAG_LIST) malloc(
			sizeof(REPLACEABLE_TAG) * 2);

	// 获取用户提交的用户名和密码，如果没有提交数据，则 username 和 password 为空字符串
	cgiFormStringSpaceNeeded("username", &field_buffer_size);

	username = (char*) malloc(field_buffer_size);
	memset(username, 0, field_buffer_size);
	cgiFormString("username", username, field_buffer_size);

	cgiFormStringSpaceNeeded("password", &field_buffer_size);
	password = (char*) malloc(field_buffer_size);
	cgiFormString("password", password, field_buffer_size);

	if (strcmp(username, g_login_username) == 0
			&& strcmp(password, g_login_password) == 0) {
		// 登录验证通过，设置 session 数据后重定向浏览器
		session_set("USERNAME", g_login_username);
		session_write_close();
		cgiHeaderLocation(cgiScriptName);
		free(username);
		free(password);
		return 0;
	}

	// 输出 ContentType header
	// 作为 CGI 程序必须输出这个 header，否则会导致 500 错误
	fprintf(cgiOut, "Pragma: no-cache\n");
	cgiHeaderContentType("text/html");

	// 显示登录页面
	tag_list[0].tag = "{%ERROR_MESSAGE%}";
	if (strlen(username) != 0 || strlen(password) != 0) {
		tag_list[0].value = "您输入的用户名或者密码错误。";
	} else {
		tag_list[0].value = "";
	}
	tag_list[1].tag = "{%USERNAME%}";
	tag_list[1].value = username;

	buffer = read_template(g_login_template);
	if (buffer == NULL) {
		fprintf(cgiOut, "Warning: Can't read template file: %s.<br />\n",
				g_login_template);
		free(username);
		free(password);
		return -1;
	}

	parse_template(&buffer, tag_list, 2);
	fprintf(cgiOut, "%s", buffer);
	free(buffer);
	free(username);
	free(password);
	// session_destroy();
	return 0;
}

int control_process() {
	char* buffer = NULL;

	// 检查用户是否按下了按钮
	if (cgiFormSubmitClicked("led1") == cgiFormSuccess) {
		// 调用 led 1 控制代码
		control_led(0);
	} else if (cgiFormSubmitClicked("led2") == cgiFormSuccess) {
		// 调用 led 2 控制代码
		control_led(1);
	} else if (cgiFormSubmitClicked("logout") == cgiFormSuccess) {
		// 退出登录
		session_destroy();
		cgiHeaderLocation(cgiScriptName);
		return 0;
	}

	// 输出 ContentType header
	// 作为 CGI 程序必须输出这个 header，否则会导致 500 错误
	fprintf(cgiOut, "Pragma: no-cache\n");
	cgiHeaderContentType("text/html");

	buffer = read_template(g_control_template);
	if (buffer == NULL) {
		fprintf(cgiOut, "Warning: Can't read template file: %s.<br />\n",
				g_control_template);
		return -1;
	}

	fprintf(cgiOut, "%s", buffer);
	free(buffer);

	return 0;
}

/**
 * 这个函数读取一个模版文件到缓冲区中，并返回缓冲区指针
 *
 * @param const char* template_filename 模版文件名
 *
 * @return char* 缓冲区指针，如果读取模版文件失败，则返回 NULL
 */
char* read_template(const char* template_filename) {
	FILE* fp = NULL;
	size_t block_size = 1024;
	size_t buffer_size = block_size;
	size_t length = 0;
	size_t read_size = 0;
	char* buffer = malloc(buffer_size);
	char* tmp_buffer = buffer;

	fp = fopen(template_filename, "rb");
	if (fp != NULL) {
		memset(buffer, 0, buffer_size);
		while ((read_size = fread(tmp_buffer, 1, block_size, fp)) != 0) {
			length += read_size;
			if (length >= buffer_size) {
				// 如果读取的文件长度已经达到了缓冲区的长度，则需要扩展缓冲区
				buffer_size += block_size;
				tmp_buffer = (char*) malloc(buffer_size);
				memset(tmp_buffer, 0, buffer_size);
				memcpy(tmp_buffer, buffer, length);
				free(buffer);
				buffer = tmp_buffer;
				tmp_buffer = buffer + length;
			} else {
				break;
			}
		}
		fclose(fp);
	}

	return buffer;
}

/**
 * 在模版内容缓冲区中使用正则表达式进行替换
 *
 * 由于替换结果有可能超过缓冲区长度，因此 template_content 参数以指针的地址形式传递。
 *
 * @param char** template_content 模版缓冲区指针的地址
 * @param REPLACEABLE_TAG_LIST tag_list 要替换的标记列表
 * @param int tag_count 标记列表包含的项目总数
 *
 * @return int 成功替换的标记
 */
int parse_template(char** template_content, REPLACEABLE_TAG_LIST tag_list,
		int tag_count) {
	char* buffer = *template_content;
	char* tmp_buffer = NULL;
	char* pos = buffer;
	size_t tag_length = 0;
	size_t value_length = 0;
	size_t length = 0;
	size_t buffer_size = 0;
	int count = 0;

	while (tag_count) {
		do {
			tag_length = strlen(tag_list[tag_count - 1].tag);
			value_length = strlen(tag_list[tag_count - 1].value);
			if (tag_length == 0) {
				break;
			}

			pos = strstr(buffer, tag_list[tag_count - 1].tag);
			if (pos == NULL) {
				break;
			}

			if (value_length <= tag_length) {
				// 不需要重新分配内存
				length = strlen(pos) - tag_length;
				strcpy(pos, tag_list[tag_count - 1].value);
				memmove(pos + value_length, pos + tag_length,
						length * sizeof(char) + 1);
				pos += value_length;
				break;
			}

			// 需要重新分配内存
			// 计算新缓冲的大小并分配内存
			length = strlen(buffer);
			buffer_size = (length + (value_length - tag_length) + 1)
					* sizeof(char);
			tmp_buffer = (char*) malloc(buffer_size);
			memset(tmp_buffer, 0, buffer_size);

			// 构造新字符串
			length = pos - buffer;
			strncpy(tmp_buffer, buffer, length);
			strcpy(tmp_buffer + length, tag_list[tag_count - 1].value);
			length += value_length;
			strcpy(tmp_buffer + length, pos + tag_length);

			// 释放原有的内存
			free(buffer);
			buffer = tmp_buffer;
		} while (0);

		tag_count--;
	}
	*template_content = buffer;

	return count;
}

/**
 * 控制 led
 */
void control_led(unsigned char number) {
	return;
}
