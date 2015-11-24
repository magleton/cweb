#include "../interface/upload.h"
char * uploadFile(char *file_name, char *error_msg) {
	cgiFilePtr source_file;
	int targetFile;
	mode_t mode;
	char fileName[128];
	char fileNameOnServer[128];
	char contentType[1024];
	char buffer[BufferLen];
	char *tmpStr = NULL;
	int size, got, t;
	if (cgiFormFileName("file", fileName, sizeof(fileName)) != cgiFormSuccess) {
		//fprintf(cgiOut, "could not retrieve filename\n");
		strcpy(error_msg, "could not retrieve filename");
		goto FAIL;
	}
	cgiFormFileSize("file", &size);
	cgiFormFileContentType("file", contentType, sizeof(contentType));
	if (cgiFormFileOpen("file", &source_file) != cgiFormSuccess) {
		//fprintf(cgiOut, "%s", "Could not open the file\n");
		strcpy(error_msg, "Could not open the file");
		goto FAIL;
	}
	t = -1;
	while (1) {
		tmpStr = strstr(fileName + t + 1, "\\");
		if (NULL == tmpStr) {
			tmpStr = strstr(fileName + t + 1, "/");
		}
		if (NULL != tmpStr) {
			t = (int) (tmpStr - fileName);
		} else {
			break;
		}
	}
	strcpy(fileNameOnServer, "../resource/upload/");
	strcat(fileNameOnServer, fileName + t + 1);
	mode = S_IRWXU | S_IRGRP | S_IROTH;
	targetFile = open(fileNameOnServer,
	O_RDWR | O_CREAT | O_TRUNC | O_APPEND, mode);
	if (targetFile < 0) {
		//fprintf(cgiOut, "could not create the new file%s", fileNameOnServer);
		char tmpStr[200] = { 0 };
		sprintf(tmpStr, "could not create the new file%s", fileNameOnServer);
		strcpy(error_msg, tmpStr);
		goto FAIL;
	}
	while (cgiFormFileRead(source_file, buffer, BufferLen, &got)
			== cgiFormSuccess) {
		if (got > 0) {
			write(targetFile, buffer, got);
		}
	}
	cgiFormFileClose(source_file);
	close(targetFile);
	goto END;
	FAIL:
	//fprintf(cgiOut, "%s", "Failed to upload");
	return "fail";
	END:
	//fprintf(cgiOut, "%s has been uploaded", fileNameOnServer);
	strcpy(file_name, fileName);
	return "success";
}
