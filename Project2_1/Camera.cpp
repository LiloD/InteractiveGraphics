#include "Camera.h"
#include <cmath>

glm::vec3 Camera::Orientation()
{
	glm::vec3 rec(cosf(_verticalAngle)*sinf(_horizontalAngle),
		sinf(_verticalAngle),
		cosf(_verticalAngle)*cosf(_horizontalAngle));
	return rec;
}

glm::mat4 Camera::ViewMatrix()
{
	glm::mat4 rec = glm::lookAt(_position, _position+Orientation(), _up);
	return rec;
}