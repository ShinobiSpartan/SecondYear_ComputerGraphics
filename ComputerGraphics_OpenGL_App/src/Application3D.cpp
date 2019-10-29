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

#pragma region Cube Transforms
	// Set all of the positions and rotations for the Cube movement
	m_positions[0] = glm::vec3(10, 0, -10);
	m_positions[1] = glm::vec3(-10, 0, -10);
	m_rotations[0] = glm::quat(glm::vec3(0, -1, 0));
	m_rotations[1] = glm::quat(glm::vec3(0, 1, 0));
#pragma endregion

#pragma region Leg Transforms
	// Set all of the positions and rotations for the leg simulation
	m_hipFrames[0].position = glm::vec3(-5, 5, 0);
	m_hipFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_hipFrames[1].position = glm::vec3(-5, 5, 0);
	m_hipFrames[1].rotation = glm::quat(glm::vec3(-1, 0, 0));

	m_kneeFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[0].rotation = glm::quat(glm::vec3(1, 0, 0));
	m_kneeFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_kneeFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));

	m_ankleFrames[0].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[0].rotation = glm::quat(glm::vec3(0.5f, 0, 0));
	m_ankleFrames[1].position = glm::vec3(0, -2.5f, 0);
	m_ankleFrames[1].rotation = glm::quat(glm::vec3(0, 0, 0));
#pragma endregion

#pragma region Load Shader
	// Load the Normal Map Shader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "../assets/shaders/normalmap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "../assets/shaders/normalmap.frag");

	// If loading the Shader fails, output error
	if (m_normalMapShader.link() == false) {
		printf("Normal Map Shader Error: %s\n", m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion

#pragma region Load Models
	// Load the Spear Mesh
	if (m_spearMesh.load("../assets/soulspear/soulspear.obj", true, true) == false) {
		printf("Soulspear Mesh Error! \n");
		return false;
	}

	// Set the scale and position for the Spear Mesh
	m_spearTransform =
	{
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,-2,1
	};

	// Load the Droid Mesh
	if (m_droidMesh.load("../assets/droid/BattleDroidT.obj", true, true) == false) {
		printf("Droid Error! \n");
		return false;
	}

	// Set the scale and position for the Droid Mesh
	m_droidTransform =
	{
		3,0,0,0,
		0,3,0,0,
		0,0,3,0,
		0,0,2,1
	};
#pragma endregion
	
	// Set the colour for Light 1
	m_light.diffuse = { 1, 1, 1 };
	m_light.specular = { 1, 1, 1 };

	// Set the colour for Light 2
	m_light2.diffuse = { 1, 1, 1 };
	m_light2.specular = { 1, 1, 1 };

	// Set the colour for the Ambient Light
	m_ambientLight = { 0, 0, 0 };
	
	return true;
}

void Application3D::shutdown()
{
	Gizmos::destroy();
}

void Application3D::update(float deltatime)
{
	float time = glfwGetTime();
	
	// Set the camera to rotate around the centre
	m_view = glm::lookAt(vec3(glm::cos(time * 0.5f) * 10, 10, glm::sin(time * 0.5f) *  10), glm::vec3(0,2,0), glm::vec3(0, 1, 0));

	Gizmos::clear();

	// Create square to draw grid
	Gizmos::addTransform(mat4(1));

	// Set colours for grid
	vec4 white(1);
	vec4 black(0, 0, 0, 1);

	// Draw grid into Aplication
	for (int i = 0; i < 21; ++i)
	{
		Gizmos::addLine(vec3(-10 + i, 0, 10), vec3(-10 + i, 0, -10), i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i), vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	// Rotate Light 1
	m_light.direction = glm::normalize(vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	// Set Light 2 direction
	m_light2.direction = {0.707,0.707,0};

#pragma region Animate Cube
	// Use time to animate a alue between [0, 1]
	float s = glm::cos(glfwGetTime()) * 0.5f + 0.5f;
	// Standard linear interpolation
	glm::vec3 p = (1.0f - s) * (m_positions[0] * 0.5f) + s * (m_positions[1] * 0.5f);
	// Quaternion slerp
	glm::quat r = glm::slerp(m_rotations[0], m_rotations[1], s);
	// Build a matrix
	glm::mat4 m = glm::translate(p) * glm::toMat4(r);
	
	// Draw a box
	Gizmos::addAABBFilled(p, glm::vec3(0.5f), glm::vec4(1, 0, 0, 1), &m);
#pragma endregion
	
#pragma region Animate Leg
	// Linearly interpolate hip position
	glm::vec3 pH = (1.0f - s) * (m_hipFrames[0].position * 0.5f) + s * (m_hipFrames[1].position * 0.5f);
	// Spherically interpolate hip rotation
	glm::quat rH = glm::slerp(m_hipFrames[0].rotation, m_hipFrames[1].rotation, s);
	// Update the hip bone
	m_hipBone = glm::translate(pH) * glm::toMat4(rH);
	
	// Linearly interpolate knee position
	glm::vec3 pK = (1.0f - s) * (m_kneeFrames[0].position * 0.5f) + s * (m_kneeFrames[1].position * 0.5f);
	// Spherically interpolate knee rotation
	glm::quat rK = glm::slerp(m_kneeFrames[0].rotation, m_kneeFrames[1].rotation, s);
	// Update the knee bone
	m_kneeBone = m_hipBone * (glm::translate(pK) * glm::toMat4(rK));
	
	// Linearly interpolate ankle position
	glm::vec3 pA = (1.0f - s) * (m_ankleFrames[0].position * 0.5f) + s * (m_ankleFrames[1].position * 0.5f);
	// Spherically interpolate ankle rotation
	glm::quat rA = glm::slerp(m_ankleFrames[0].rotation, m_ankleFrames[1].rotation, s);
	// Update the ankle bone
	m_ankleBone = m_kneeBone * (glm::translate(pA) * glm::toMat4(rA));

	// Set positions for Leg
	glm::vec3 hipPos = glm::vec3(m_hipBone[3].x, m_hipBone[3].y, m_hipBone[3].z);
	glm::vec3 kneePos = glm::vec3(m_kneeBone[3].x, m_kneeBone[3].y, m_kneeBone[3].z);
	glm::vec3 anklePos = glm::vec3(m_ankleBone[3].x, m_ankleBone[3].y, m_ankleBone[3].z);
	
	// Set Colour for the Leg
	glm::vec4 half(0.5f);
	glm::vec4 pink(1, 0, 1, 1);
	
	// Draw the Leg using cubes
	Gizmos::addAABBFilled(hipPos, half, pink, &m_hipBone);
	Gizmos::addAABBFilled(kneePos, half, pink, &m_kneeBone);
	Gizmos::addAABBFilled(anklePos, half, pink, &m_ankleBone);
#pragma endregion
}

void Application3D::draw()
{
	clearScreen();

	// Bind Normal Map Shader
	m_normalMapShader.bind();

	// Bind Ambient Light
	m_normalMapShader.bindUniform("Ia", m_ambientLight);

	// Bind Light 1 in the Shader
	m_normalMapShader.bindUniform("Id", m_light.diffuse);
	m_normalMapShader.bindUniform("Is", m_light.specular);
	m_normalMapShader.bindUniform("lightDirection", m_light.direction);

	// Bind Light 2 in the Shader
	m_normalMapShader.bindUniform("Jd", m_light2.diffuse);
	m_normalMapShader.bindUniform("Js", m_light2.specular);
	m_normalMapShader.bindUniform("light2Direction", m_light2.direction);
	
	m_normalMapShader.bindUniform("cameraPosition", vec3(glm::inverse(m_view)[3]));

	// Bind transform (Soulspear)
	auto pvm = m_projection * m_view * m_spearTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	
	// Bind transforms for lighting (Soulspear)
	m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_spearTransform)));
	
	// Draw Spear
	m_spearMesh.draw();

	// ---------------------------------------------------------------

	// Bind transform (Droid)
	pvm = m_projection * m_view * m_droidTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);

	// Bind transforms for lighting (Droid)
	m_normalMapShader.bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_droidTransform)));

	// Draw Droid
	m_droidMesh.draw();

	// ---------------------------------------------------------------

	// Output everything to the Application
	aie::Gizmos::draw(m_projection * m_view);
}
