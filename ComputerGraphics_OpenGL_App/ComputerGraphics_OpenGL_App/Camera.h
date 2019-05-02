#pragma once

#include <glm/ext.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	virtual void update(float deltaTime) = 0;

	void setPerspective(float fieldOfView, float aspectRatio, float near, float far);
	void setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up);
	void setPosition(glm::vec3 position);
	
	glm::mat4 getWorldTransform() { return m_worldTransform; }
	glm::mat4 getView() { return m_viewTransform; }
	glm::mat4 getProjection() { return m_projectionTransform; }
	glm::mat4 getProjectionView() { return m_projectionViewTransform; }

private:
	void updateProjectionViewTransform();

	glm::mat4	m_worldTransform;
	glm::mat4	m_viewTransform;
	glm::mat4	m_projectionTransform;
	glm::mat4	m_projectionViewTransform;
	
};

