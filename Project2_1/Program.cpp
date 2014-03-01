#include "Program.h"

Program::Program(std::vector<GLuint> shaderIds)
{
	_id = glCreateProgram();
	for (int i = 0; i < shaderIds.size(); ++i)
	{
		glAttachShader(_id,shaderIds[i]);
	}
	glLinkProgram(_id);

	// Check the program
	GLint Result = GL_FALSE;
	int InfoLogLength;
	glGetProgramiv(_id, GL_LINK_STATUS, &Result);
	glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(_id, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
}

Program::~Program()
{
	glDeleteProgram(_id);
}


void Program::SetUniform(const char * name, int value)
{
	glUniform1i(glGetUniformLocation(_id,name),value);
}
void Program::SetUniform(const char * name, glm::mat4 mat4_value)
{
	glUniformMatrix4fv(glGetUniformLocation(_id,name),1,GL_FALSE, glm::value_ptr(mat4_value));
}