#include "camera.h"


Camera::Camera() : m_position(0.0f, 0.0f, 0.0f), m_azimuthal_angle(-90.0f), m_polar_angle(0.0f)
{

}

Camera::Camera(glm::vec3 position) : m_position(position), m_azimuthal_angle(-90.0f), m_polar_angle(0.0f)
{

}

Camera::Camera(glm::vec3 position, float azimuthal_angle, float polar_angle) 
	: m_position(position), m_azimuthal_angle(azimuthal_angle), m_polar_angle(polar_angle)
{

}

// Process mouse movement (drag & move). Update direction of camera.
void Camera::processMouseMovement(float x_offset, float y_offset)
{
	x_offset *= m_SENSITIVITY;
	y_offset *= m_SENSITIVITY;

	m_azimuthal_angle += x_offset;
	m_polar_angle += y_offset;

	if (m_polar_angle >= 90.0f) m_polar_angle = 90.0f;
	else if (m_polar_angle <= -90.0f) m_polar_angle = -90.0f;
}

// Process mouse scroll. Update FOV for zoom effect
void Camera::processMouseScroll(float scroll_amt)
{
	m_fov -= scroll_amt;
	if (m_fov < 1.0f) m_fov = 1.0f;
	else if (m_fov > 45.0f) m_fov = 45.0f;
}

// Setters
void Camera::setPosition(const glm::vec3 &new_pos)
{
	m_position = new_pos;
}

void Camera::setFOV(float new_fov)
{
	m_fov = new_fov;
}
void Camera::setAspectRatio(float new_ar)
{
	m_aspect_ratio = new_ar;
}

// Getters
glm::vec3 Camera::getPosition()
{
	return m_position;
}

float Camera::getAzimuthalAngle()
{
	return m_azimuthal_angle;
}

float Camera::getPolarAngle()
{
	return m_polar_angle;
}

float Camera::getFOV()
{
	return m_fov;
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
	dest = glm::lookAt(m_position, target, up);
}

// Calc Projection matrix
glm::mat4 Camera::getProjMatrix()
{
	glm::mat4 proj_matrix;
	this->getProjMatrix(proj_matrix);

	return proj_matrix;
}

void Camera::getProjMatrix(glm::mat4 &dest)
{
	dest = glm::perspective(glm::radians(m_fov), m_aspect_ratio, 0.1f, 100.0f);
}
