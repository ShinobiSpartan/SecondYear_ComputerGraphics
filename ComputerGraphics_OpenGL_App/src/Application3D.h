#pragma once
#include "Shader.h"
#include "OBJMesh.h"
#include "Application.h"
#include "Mesh.h"

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

	//Mesh		m_quad;
	//Mesh		m_quadMesh;
	//glm::mat4	m_quadTransform;


	// ---------------------------------------------------------------

	// Leg
	glm::vec3		m_positions[2];
	glm::quat		m_rotations[2];

	KeyFrame m_hipFrames[2];
	KeyFrame m_kneeFrames[2];
	KeyFrame m_ankleFrames[2];

	glm::mat4 m_hipBone;
	glm::mat4 m_kneeBone;
	glm::mat4 m_ankleBone;

	// ---------------------------------------------------------------

	aie::ShaderProgram	m_shader;
	
	aie::ShaderProgram	m_phongShader;

	aie::ShaderProgram	m_texturedShader;

	// ---------------------------------------------------------------

	// Dragon Model
	aie::OBJMesh		m_dragonMesh;
	glm::mat4			m_dragonTransform;

	// Soulspear Model
	aie::OBJMesh		m_spearMesh;
	glm::mat4			m_spearTransform;

	// ---------------------------------------------------------------

	struct Light 
	{
		glm::vec3 direction;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};

	Light	m_light;
	glm::vec3 m_ambientLight;
};

