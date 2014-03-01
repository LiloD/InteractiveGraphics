#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject()
{
	_id = 0;
	_size = 0;
	_bufferTarget = GL_ARRAY_BUFFER;
	std::vector<unsigned char> _data;
	_isUpload = false;
}


VertexBufferObject::~VertexBufferObject()
{
	releaseVBO();
}


void VertexBufferObject::CreateVBO()
{
	glGenBuffers(1,&_id);
}


void VertexBufferObject::releaseVBO()
{
	glDeleteBuffers(1,&_id);
	_id = 0;
	_size = 0;
	_data.clear();
	_isUpload = false; 
}


void VertexBufferObject::bindVBO(GLenum bufferTarget)
{
	_bufferTarget = bufferTarget;
	glBindBuffer(_bufferTarget, _id);
}


void VertexBufferObject::uploadDataToGPU(int usage)
{
	glBufferData(_bufferTarget, _size , &_data[0], usage);
	_data.clear();
	_isUpload = true; 
}


void VertexBufferObject::addData(void* data, unsigned int dataSize)
{
	_data.insert(_data.end(),(unsigned char*)data, (unsigned char*)data+dataSize);
	_size += dataSize;
}