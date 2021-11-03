#include "camera.h"


Camera::Camera() : position(0.0f, 0.0f, 0.0f), azimuthal_angle(-90.0f), polar_angle(0.0f)
{

}

Camera::Camera(glm::vec3 position) : position(position), azimuthal_angle(-90.0f), polar_angle(0.0f)
{

}

Camera::Camera(glm::vec3 position, float azimuthal_angle, float polar_angle) 
	: position(position), azimuthal_angle(azimuthal_angle), polar_angle(polar_angle)
{

}

// Process mouse movement (drag & move). Update direction of camera.
void Camera::processMouseMovement(float x_offset, float y_offset)
{
	x_offset *= this->SENSITIVITY;
	y_offset *= this->SENSITIVITY;

	this->azimuthal_angle += x_offset;
	this->polar_angle += y_offset;

	if (this->polar_angle >= 90.0f) this->polar_angle = 90.0f;
	else if (this->polar_angle <= -90.0f) this->polar_angle = -90.0f;
}

// Process mouse scroll. Update FOV for zoom effect
void Camera::processMouseScroll(float scroll_amt)
{
	this->fov -= scroll_amt;
	if (this->fov < 1.0f) this->fov = 1.0f;
	else if (this->fov > 45.0f) this->fov = 45.0f;
}

// Setters
void Camera::setPosition(const glm::vec3 &new_pos)
{
	this->position = new_pos;
}

void Camera::setFOV(float new_fov)
{
	this->fov = new_fov;
}
void Camera::setAspectRatio(float new_ar)
{
	this->aspect_ratio = new_ar;
}

// Getters
glm::vec3 Camera::getPosition()
{
	return this->position;
}

float Camera::getAzimuthalAngle()
{
	return this->azimuthal_angle;
}

float Camera::getPolarAngle()
{
	return this->polar_angle;
}

glm::vec3 Camera::getFOV()
{
	return this->fov;
}

// Calc View matrix using LookAt
glm::mat4 Camera::getViewMatrix()
{
	glm::mat4 view_matrix;
	this->getViewMatrix(view_matrix);

	return view_matrix;
}

void Camera::getViewMatrix(glm::mat4 &dest)
{
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 target(0.0f, 0.0f, 0.0f);
	dest = glm::lookAt(this->position, target, up);
}

// Calc Projection matrix
glm::mat4 Camera::getProjMatrix()
{
	glm::mat4 proj_matrix;
	this->getProjhMatrix(proj_matrix);

	return proj_matrix;
}

void Camera::getProjMatrix(glm::mat4 &dest)
{
	dest = glm::perspective(glm::radians(this->fov), this->ASPECT_RATIO, 0.1f, 100.0f);
}
