#include "Shader.h"
#include <iostream>


Shader::Shader(const char * filePath, GLenum shaderType)
{
	_id = glCreateShader(shaderType);

	std::string ShaderSrc;
	std::fstream shaderFile(filePath, std::ios::in);
	if(shaderFile.is_open())
	{
		std::string line;
		while(std::getline(shaderFile,line))
		{
			ShaderSrc += "\n" + line;
		}
		shaderFile.close();
	}

	const char* ShaderSrcPtr = ShaderSrc.c_str();
	glShaderSource(_id,1,&ShaderSrcPtr,NULL);
	glCompileShader(_id);

	// Check Fragment Shader
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetShaderiv(_id, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(_id, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
}

Shader::Shader(const Shader& other)
{
	_id = other._id;
}

Shader& Shader::operator =(const Shader& other)
{
	_id = other._id;
	return *this;
}
Shader::~Shader()
{
	glDeleteShader(_id);
}