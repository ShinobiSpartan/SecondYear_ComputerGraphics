#pragma once
#include "Shader.h"
#include "OBJMesh.h"
#include "Application.h"

struct KeyFrame {
	glm::vec3 position;
	glm::quat rotation;
};

class Application3D : public Application
{
public:
	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltatime);
	virtual void draw();

protected:
	glm::vec3		m_positions[2];
	glm::quat		m_rotations[2];

	KeyFrame m_hipFrames[2];
	KeyFrame m_kneeFrames[2];
	KeyFrame m_ankleFrames[2];

	glm::mat4 m_hipBone;
	glm::mat4 m_kneeBone;
	glm::mat4 m_ankleBone;


	aie::ShaderProgram	m_shader;
	aie::ShaderProgram	m_phongShader;
	aie::OBJMesh		m_bunnyMesh;
	glm::mat4			m_bunnyTransform;

	struct Light 
	{
		glm::vec3 direction;
	};
	Light	m_light;
};

