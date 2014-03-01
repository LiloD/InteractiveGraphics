#ifndef _TRANSFORM_H
#define _TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL\glew.h>

class Transform{
public:
	// convenience function that returns a translation matrix
	static glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z) {
		return glm::translate(glm::mat4(), glm::vec3(x,y,z));
	}

	// convenience function that returns a scaling matrix
	static glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z){
		return glm::scale(glm::mat4(), glm::vec3(x,y,z));
	}
};


#endif //_TRANSFORM_H