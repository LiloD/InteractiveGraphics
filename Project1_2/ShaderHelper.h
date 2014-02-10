#ifndef SHADER_HELPER_H
#define SHADER_HELPER_H

#include <iostream>
#include <cstddef>
#include <cstdio>
#include <GL\glew.h>
#include <GL\freeglut.h>

using namespace std;

struct ShaderInfo;
typedef struct ShaderInfo* ShadersInfo;
char* readFromFile(const char* shaderFile);
GLuint loadShader(ShadersInfo,int);


struct ShaderInfo
{
	GLenum Type;
	char* filename;
};


char* readFromFile(const char* shaderFileName)
{
	FILE* file = fopen(shaderFileName,"r");
	if(file == NULL)
	{
		return NULL;
	}

	char* buff;
	long size;

	//Move the r/w pointer to the end of the file 
	fseek(file,0L,SEEK_END);
	//Caculate the size from the beginning of the file to the end
	size = ftell(file);
	//Allocate size+1 for the buff in order to store a NULL termination
	buff = (char* )malloc(sizeof(char)*(size+1));
	if(buff == NULL)
	{
		return  NULL;
	}

	//Move the pointer back to the beginning
	fseek(file,0L,SEEK_SET);
	fread(buff, 1, size,file);

	//Set the NULL Termination
	buff[size] = '\0';

	fclose(file);
	return buff;
}


GLuint loadShader(ShadersInfo shaderInfo, int count)
{
	GLenum ErrorCheckValue = glGetError();

	GLuint shaderProgram = glCreateProgram();
	for(int i = 0; i< count; ++i)
	{
		GLchar* shaderbuf = readFromFile(shaderInfo[i].filename);

		GLuint shader = glCreateShader(shaderInfo[i].Type);
		glShaderSource(shader,1, (const GLchar**)&shaderbuf,NULL);
		glCompileShader(shader);
		
		glAttachShader(shaderProgram,shader);

		free(shaderbuf);
	}
	glLinkProgram(shaderProgram);
	//glUseProgram(shaderProgram);
	
	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(ErrorCheckValue)
		);

		exit(-1);
	}
	return shaderProgram;
}


#endif //SHADER_HELPER_H