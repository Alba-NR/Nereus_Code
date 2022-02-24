#ifndef CAMERA
#define CAMERA
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../main/constants.h"

enum class CameraMovement
{
	FORWARDS	= 0,
	BACKWARDS	= 1,
	LEFT		= 2,
	RIGHT		= 3,
	UPWARDS		= 4,
	DOWNWARDS	= 5
};

class Camera
{
private:
	glm::vec3 m_position;
	float m_azimuthal_angle;
	float m_polar_angle;
	
	float m_fov = 45.0f;
	float m_aspect_ratio = NereusConstants::DEFAULT_WINDOW_WIDTH / NereusConstants::DEFAULT_WINDOW_HEIGHT;
	const float m_SPEED = NereusConstants::DEFAULT_CAMERA_SPEED;
	const float m_SENSITIVITY = NereusConstants::DEFAULT_CAMERA_SENSITIVITY;

public:
	Camera();
	Camera(glm::vec3 position);
	Camera(glm::vec3 position, float azimuthal_angle, float polar_angle);

	void move(CameraMovement direction, float delta_time);
	void processMouseMovement(float x_offset, float y_offset);
	void processMouseScroll(float scroll_amt);

	void setPosition(const glm::vec3 &new_pos);
	void setAzimuthalAngle(float new_azimuthal);
	void setPolarAngle(float new_polar);
	void setFOV(float new_fov);
	void setAspectRatio(float new_ar);

	glm::vec3 getPosition() const;
	float getAzimuthalAngle() const;
	float getPolarAngle() const;
	float getFOV() const;
	glm::vec3 Camera::getFrontVector() const;

	glm::mat4 getViewMatrix() const;
	void getViewMatrix(glm::mat4 &dest) const;

	glm::mat4 getProjMatrix() const;
	void getProjMatrix(glm::mat4 &dest) const;
};

#endif