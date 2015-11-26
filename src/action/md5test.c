/*
 * ooptest.c
 *
 *  Created on: 2015年11月18日
 *  Author: macro
 */

#include <stdio.h>
#include <string.h>
#include "../../lib/cgic/cgic.h"
#include "../model/interface/md5.h"
int cgiMain() {
	cgiHeaderContentType("text/html;charset=UTF-8");
	MD5_CTX md5;
	MD5Init(&md5);
	int i;
	unsigned char encrypt[] = "admin"; //21232f297a57a5a743894a0e4a801fc3
	unsigned char decrypt[16];
	MD5Update(&md5, encrypt, strlen((char *) encrypt));
	MD5Final(&md5, decrypt);
	fprintf(cgiOut , "加密前:%s\n加密后16位:", encrypt);
	for (i = 4; i < 12; i++) {
		fprintf(cgiOut , "%02x", decrypt[i]);  //02x前需要加上 %
	}

	fprintf(cgiOut , "\n加密前:%s\n加密后32位:", encrypt);
	for (i = 0; i < 16; i++) {
		fprintf(cgiOut , "%02x", decrypt[i]);  //02x前需要加上 %
	}
	return 0;
}
