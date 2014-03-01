#ifndef _VERTEX_BUFFER_OBJECT_H
#define _VERTEX_BUFFER_OBJECT_H
#include <GL\glew.h>
#include <vector>


class VertexBufferObject
{
public:
	VertexBufferObject();
	~VertexBufferObject();
	void CreateVBO();
	void releaseVBO();
	void bindVBO(GLenum bufferTarget = GL_ARRAY_BUFFER);
	void uploadDataToGPU(int usage);
	void addData(void* ptrData, unsigned int uiDataSize);

	GLuint getId()
	{
		return _id;
	}

private:
	GLuint _id;
	unsigned int _size;
	GLenum _bufferTarget;
	std::vector<unsigned char> _data;
	bool _isUpload;
};




#endif //_VERTEX_BUFFER_OBJECT_H