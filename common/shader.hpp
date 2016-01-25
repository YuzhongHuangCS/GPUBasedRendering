/*
* shader.cpp
*
* shader and program managment utility
*/

#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

#define glCheckError() _glCheckError(__FILE__, __LINE__)
GLenum _glCheckError(char* file, int line) {
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) fprintf(stderr, "glError in file %s @ line %d: %s\n", file, line, gluErrorString(error));
	return error;
}

GLuint glBuildShader(char* shaderCode, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &result);
		char* info = new char[result];
		glGetShaderInfoLog(shader, result, NULL, info);
		fprintf(stderr, "Build shader failed: %s\n", info);
		delete info;
	}

	return shader;
}

GLuint glBuildProgram(char** shaderCodeArray, GLenum* shaderTypeArray, GLuint length) {
	GLuint program = glCreateProgram();
	for (GLuint i = 0; i < length; i++) {
		glAttachShader(program, glBuildShader(shaderCodeArray[i], shaderTypeArray[i]));
		if (shaderTypeArray[i] == GL_GEOMETRY_SHADER) {
			glProgramParameteriEXT(program, GL_GEOMETRY_INPUT_TYPE_EXT, GL_TRIANGLES);
			glProgramParameteriEXT(program, GL_GEOMETRY_OUTPUT_TYPE_EXT, GL_TRIANGLES);

			GLint maxOut;
			glGetIntegerv(GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT, &maxOut);
			glProgramParameteriEXT(program, GL_GEOMETRY_VERTICES_OUT_EXT, maxOut);
		}
	}
	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &result);
		char* info = new char[result];
		glGetProgramInfoLog(program, result, NULL, info);
		fprintf(stderr, "Build program failed: %s\n", info);
		delete info;
	}

	return program;
}

#endif // SHADER_H