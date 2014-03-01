#ifndef _CAMERA_H
#define _CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera():_position(0.0f,0.0f,3.0f),
			_horizontalAngle(3.14f),
			_verticalAngle(0.0f),
			_up(glm::vec3(0.0f,1.0f,0.0f))
	{
		_forward = Orientation();
		_right = glm::normalize(glm::cross(_forward,_up));
		//_up = glm::normalize(glm::cross(_right,_forward));
	}

	glm::vec3 Orientation();

	glm::mat4 ViewMatrix();

	glm::vec3 right()
	{
		return _right;
	}

	glm::vec3 forward()
	{
		return _forward;
	}

	glm::vec3 up()
	{
		return _up;
	}

	glm::vec3 getPosition()
	{
		return _position;
	}

	void setPosition(glm::vec3 p)
	{
		_position = p;
	}

	void setHorizontalAngle(float a)
	{
		_horizontalAngle = a;
		_forward =  Orientation();
		_right =  glm::normalize(glm::cross(_forward,_up));
	}

	float getHorizontalAngle()
	{
		return _horizontalAngle;
	}

	void setVerticalAngle(float a)
	{
		_verticalAngle = a;
		_forward =  Orientation();
		//_up =  glm::normalize(glm::cross(_right,_forward));
	}

	float getVerticalAngle()
	{
		return _verticalAngle;
	}

private:
	glm::vec3 _position;
	float _horizontalAngle;
	float _verticalAngle;
	glm::vec3 _up;
	glm::vec3 _forward;
	glm::vec3 _right;
};

#endif