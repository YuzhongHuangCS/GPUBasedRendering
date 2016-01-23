#pragma once

/*
* textfile.hpp
*
* simple reading and writing for text files
*/

#include <iostream>

char* textFileRead(char *path) {
	FILE* fp = NULL;
	char* content = NULL;

	fp = fopen(path, "rt");
	if (fp != NULL) {
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		rewind(fp);

		content = new char[size + 1];
		fread(content, size, sizeof(char), fp);
		content[size] = '\0';
		fclose(fp);
	}

	return content;
}

int textFileWrite(char* path, char* data) {
	FILE *fp = NULL;
	int status = 0;

	fp = fopen(path, "w");
	if (fp != NULL) {
		if (fwrite(data, sizeof(char), strlen(data), fp) == strlen(data)) status = 1;
		fclose(fp);
	}

	return status;
}