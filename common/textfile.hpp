/*
* textfile.hpp
*
* simple reading and writing for text files
*/

#pragma once

#ifndef TEXTFILE_H
#define TEXTFILE_H

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
	int count = 0;

	fp = fopen(path, "w");
	if (fp != NULL) {
		count = fwrite(data, sizeof(char), strlen(data), fp);
		fclose(fp);
	}

	return count;
}

#endif // TEXTFILE_H