#include "camera.h"
#include "../main/constants.h"


Camera::Camera() : m_position(0.0f, 0.0f, 0.0f), m_azimuthal_angle(-135.0f), m_polar_angle(-10.0f)
{

}

Camera::Camera(glm::vec3 position) : m_position(position), m_azimuthal_angle(-135.0f), m_polar_angle(-10.0f)
{

}

Camera::Camera(glm::vec3 position, float azimuthal_angle, float polar_angle) 
	: m_position(position), m_azimuthal_angle(azimuthal_angle), m_polar_angle(polar_angle)
{

}

// Move camera's position in given direction, based on time taken & cam's velocity.
void Camera::move(CameraMovement direction, float delta_time)
{
	float distance = m_SPEED * delta_time * 10.0f;
	glm::vec3 front = getFrontVector();

	switch (direction)
	{
	case CameraMovement::FORWARDS:
		// move along 'front' direction, but horizontally in x-z plane only
		m_position += glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * distance;
		break;
	case CameraMovement::BACKWARDS:
		// move along '-front' direction, but horizontally in x-z plane only
		m_position -= glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * distance;
		break;
	case CameraMovement::LEFT:
		// move along 'right' (= cross product of 'up' & 'front') direction
		m_position += glm::normalize(glm::cross(getFrontVector(), glm::vec3(0.0f, 1.0f, 0.0f))) * distance;
		break;
	case CameraMovement::RIGHT:
		// move along '-right' (= cross product of 'up' & 'front') direction
		m_position -= glm::normalize(glm::cross(getFrontVector(), glm::vec3(0.0f, 1.0f, 0.0f))) * distance;
		break;
	case CameraMovement::UPWARDS:
		// move along 'up' direction
		m_position += glm::vec3(0.0f, 1.0f, 0.0f) * distance;
		break;
	case CameraMovement::DOWNWARDS:
		// move along '-up' direction
		m_position -= glm::vec3(0.0f, 1.0f, 0.0f) * distance;
		break;
	default:
		break;
	}
}

// Process mouse movement (drag & move). Update direction of camera.
void Camera::processMouseMovement(float x_offset, float y_offset)
{
	x_offset *= m_SENSITIVITY;
	y_offset *= m_SENSITIVITY;

	m_azimuthal_angle += x_offset;
	m_polar_angle += y_offset;

	// limit movement
	//m_azimuthal_angle = glm::clamp(m_azimuthal_angle, -180.0f, -60.0f); // -140.0f, -90.0f
	//m_polar_angle = glm::clamp(m_polar_angle, -45.0f, 15.0f);  // -30.0f, 0.0f)
}

// Process mouse scroll. Update FOV for zoom effect
void Camera::processMouseScroll(float scroll_amt)
{
	m_fov -= scroll_amt;
	m_fov = glm::clamp(m_fov, 1.0f, 45.0f);
}

// Setters
void Camera::setPosition(const glm::vec3 &new_pos)
{
	m_position = new_pos;
}

void Camera::setAzimuthalAngle(float new_azimuthal)
{
	m_azimuthal_angle = new_azimuthal;
}

void Camera::setPolarAngle(float new_polar)
{
	m_polar_angle = new_polar;
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
glm::vec3 Camera::getPosition() const
{
	return m_position;
}

float Camera::getAzimuthalAngle() const
{
	return m_azimuthal_angle;
}

float Camera::getPolarAngle() const
{
	return m_polar_angle;
}

float Camera::getFOV() const
{
	return m_fov;
}

glm::vec3 Camera::getFrontVector() const
{
	return glm::normalize(glm::vec3(
		glm::cos(glm::radians(m_polar_angle)) * glm::cos(glm::radians(m_azimuthal_angle)),
		glm::sin(glm::radians(m_polar_angle)),
		glm::cos(glm::radians(m_polar_angle)) * glm::sin(glm::radians(m_azimuthal_angle))
	));
}

// Calc View matrix using LookAt
glm::mat4 Camera::getViewMatrix() const
{
	glm::mat4 view_matrix;
	this->getViewMatrix(view_matrix);

	return view_matrix;
}

void Camera::getViewMatrix(glm::mat4 &dest) const
{
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 target = m_position + getFrontVector();
	dest = glm::lookAt(m_position, target, up);
}

// Calc Projection matrix
glm::mat4 Camera::getProjMatrix() const
{
	glm::mat4 proj_matrix;
	this->getProjMatrix(proj_matrix);

	return proj_matrix;
}

void Camera::getProjMatrix(glm::mat4 &dest) const
{
	dest = glm::perspective(glm::radians(m_fov), m_aspect_ratio, NereusConstants::CAMERA_NEAR_PLANE, NereusConstants::CAMERA_FAR_PLANE);
}
