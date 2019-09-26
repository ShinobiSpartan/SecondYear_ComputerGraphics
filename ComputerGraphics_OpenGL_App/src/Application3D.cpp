#include "Application3D.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

using aie::Gizmos;
using glm::vec3;
using glm::vec4;
using glm::mat4;


Application3D::Application3D()
{
}


Application3D::~Application3D()
{
}

bool Application3D::startup()
{
	Gizmos::create(65535U, 65535U, 255U, 255U);

	// Single cube
	//m_positions[0] = glm::vec3(10, 0, -10);
	//m_positions[1] = glm::vec3(-10, 0, -10);
	//m_rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	//m_rotations[1] = glm::quat(glm::vec3(0, 1, 0));

	// Leg
	//m_hipFrames[0].position = glm::vec3(-5, 5, 0);
	//m_hipFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	//m_hipFrames[1].position = glm::vec3(-5, 5, 0);
	//m_hipFrames[1].rotation = glm::quat(glm::vec3(-1, 0, 0));
	
	//m_kneeFrames[0].position = glm::vec3(0, -2.5f, 0);
	//m_kneeFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	//m_kneeFrames[1].position = glm::vec3(0, -2.5f, 0);
	//m_kneeFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));
	
	//m_ankleFrames[0].position = glm::vec3(0, -2.5f, 0);
	//m_ankleFrames[0].rotation = glm::quat(glm::vec3(0.5f, 0, 0));
	//m_ankleFrames[1].position = glm::vec3(0, -2.5f, 0);
	//m_ankleFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));


	m_shader.loadShader(aie::eShaderStage::VERTEX, "../assets/shaders/simple.vert");
	m_shader.loadShader(aie::eShaderStage::FRAGMENT, "../assets/shaders/simple.frag");

	if (m_shader.link() == false) {
		printf("Shader Error: %s\n", m_shader.getLastError());
		return false;
	}
	
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "../assets/shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "../assets/shaders/phong.frag");

	if (m_phongShader.link() == false) {
		printf("Phong Shader Error: %s\n", m_phongShader.getLastError());
		return false;
	}

	if (m_gridTexture.load("./textures/numbered_grid.tga") == false) {
		printf("Grid Texture Error! \n");
		return false;
	}

	// Create simnple quad
	m_quad

	if (m_dragonMesh.load("../assets/stanford/dragon.obj") == false) {
		printf("Dragon Mesh Error! \n");
		return false;
	}

	m_dragonTransform =
	{
		0.5f, 0, 0, 0,
		0, 0.5f, 0, 0,
		0, 0, 0.5f, 0,
		0, 0, 0, 1.0f
	};

	m_light.diffuse = { 1,0,0 };
	m_light.specular = { 1,0,0 };
	m_ambientLight = { 0.25f,0.25f,0.25f };
	
	return true;
}

void Application3D::shutdown()
{
	Gizmos::destroy();
}

void Application3D::update(float deltatime)
{
	Gizmos::clear();

	Gizmos::addTransform(mat4(1));

	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	// Query time since application started
	float time = glfwGetTime();

	// Rotate light
	m_light.direction = glm::normalize(vec3(glm::cos(time * 2), glm::sin(time * 2), 0));


	// Cube
	// // Use time to animate a alue between [0, 1]
	// float s = glm::cos(glfwGetTime()) * 0.5f + 0.5f;
	// // Standard linear interpolation
	// glm::vec3 p = (1.0f - s) * (m_positions[0] * 0.5f) + s * (m_positions[1] * 0.5f);
	// // Quaternion slerp
	// glm::quat r = glm::slerp(m_rotations[0], m_rotations[1], s);
	// // Build a matrix
	// glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	// 
	// // Draw a box
	// Gizmos::addAABBFilled(p, glm::vec3(0.5f), glm::vec4(1, 0, 0, 1), &m);


	// Leg
	// // Linearly interpolate hip position
	// glm::vec3 pH = (1.0f - s) * (m_hipFrames[0].position * 0.5f) + s * (m_hipFrames[1].position * 0.5f);
	// // Spherically interpolate hip rotation
	// glm::quat rH = glm::slerp(m_hipFrames[0].rotation, m_hipFrames[1].rotation, s);
	// // Update the hip bone
	// m_hipBone = glm::translate(pH) * glm::toMat4(rH);
	// 
	// // Linearly interpolate knee position
	// glm::vec3 pK = (1.0f - s) * (m_kneeFrames[0].position * 0.5f) + s * (m_kneeFrames[1].position * 0.5f);
	// // Spherically interpolate knee rotation
	// glm::quat rK = glm::slerp(m_kneeFrames[0].rotation, m_kneeFrames[1].rotation, s);
	// // Update the knee bone
	// m_kneeBone = m_hipBone * (glm::translate(pK) * glm::toMat4(rK));
	// 
	// // Linearly interpolate ankle position
	// glm::vec3 pA = (1.0f - s) * (m_ankleFrames[0].position * 0.5f) + s * (m_ankleFrames[1].position * 0.5f);
	// // Spherically interpolate ankle rotation
	// glm::quat rA = glm::slerp(m_ankleFrames[0].rotation, m_ankleFrames[1].rotation, s);
	// // Update the ankle bone
	// m_ankleBone = m_kneeBone * (glm::translate(pA) * glm::toMat4(rA));

	// Draw Leg
	// glm::vec3 hipPos = glm::vec3(m_hipBone[3].x, m_hipBone[3].y, m_hipBone[3].z);
	// glm::vec3 kneePos = glm::vec3(m_kneeBone[3].x, m_kneeBone[3].y, m_kneeBone[3].z);
	// glm::vec3 anklePos = glm::vec3(m_ankleBone[3].x, m_ankleBone[3].y, m_ankleBone[3].z);
	// 
	// glm::vec4 half(0.5f);
	// glm::vec4 pink(1, 0, 1, 1);
	// 
	// Gizmos::addAABBFilled(hipPos, half, pink, &m_hipBone);
	// Gizmos::addAABBFilled(kneePos, half, pink, &m_kneeBone);
	// Gizmos::addAABBFilled(anklePos, half, pink, &m_ankleBone);
}

void Application3D::draw()
{
	clearScreen();

	// Bind phong shader
	m_phongShader.bind();

	// Bind light
	m_phongShader.bindUniform("Ia", m_ambientLight);
	m_phongShader.bindUniform("Id", m_light.diffuse);
	m_phongShader.bindUniform("Is", m_light.specular);
	m_phongShader.bindUniform("LightDirection", m_light.direction);

	m_phongShader.bindUniform("cameraPosition", vec3(glm::inverse(m_view)[3]));

	// ---------------------------------------------------------------

	// // Bind transform (Dragon)
	// auto pvm = m_projection * m_view * m_dragonTransform;
	// m_phongShader.bindUniform("ProjectionViewModel", pvm);
	// 
	// // Bind transforms for lighting (Dragon)
	// m_phongShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_dragonTransform)));
	// 
	// // Draw Dragon
	// m_dragonMesh.draw();

	// ---------------------------------------------------------------

	aie::Gizmos::draw(m_projection * m_view);
}
