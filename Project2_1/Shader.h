#ifndef _SHADER_H
#define _SHADER_H

#include <GL\glew.h>
#include <string>
#include <fstream>
#include <vector>

class Shader
{
public:
	Shader(const char * filePath, GLenum shaderType);
	Shader(const Shader&);
	Shader& operator =(const Shader& other);
	~Shader();
	GLuint getId()
	{
		return _id;
	}
private:
	GLuint _id;

};



#endif