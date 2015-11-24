/*
 * upload.h
 *
 *  Created on: 2015年11月24日
 *      Author: macro
 */

#ifndef SRC_MODEL_IMPL_UPLOAD_H_
#define SRC_MODEL_IMPL_UPLOAD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "../../../lib/cgic/cgic.h"
#include "../../../lib/ctemplate/ctemplate.h"
#include "../../../lib/oop/lw_new.h"

#define BufferLen 1024

int uploadFile(char *file_name , char  *error_msg);

#endif /* SRC_MODEL_IMPL_UPLOAD_H_ */
