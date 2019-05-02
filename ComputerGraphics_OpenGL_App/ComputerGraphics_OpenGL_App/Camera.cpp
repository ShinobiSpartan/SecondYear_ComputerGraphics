#include "Camera.h"



Camera::Camera()
{

}

Camera::~Camera()
{
}

void Camera::setPerspective(float fieldOfView, float aspectRatio, float near, float far)
{
	m_projectionTransform = glm::perspective(fieldOfView, aspectRatio, near, far);
}

void Camera::setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	m_viewTransform = glm::lookAt(from, to, up);
	m_worldTransform = glm::inverse(m_viewTransform);
}

void Camera::setPosition(glm::vec3 position)
{
	m_worldTransform[3] = glm::vec4(position, 1);
	m_viewTransform = glm::inverse(m_worldTransform);
}

void Camera::updateProjectionViewTransform()
{
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
}
