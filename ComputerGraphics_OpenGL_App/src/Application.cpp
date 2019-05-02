#include "Application.h"
#include <gl_core_4_4.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Gizmos.h>


Application::Application() : m_window(nullptr), m_gameOver(false)
{
}


Application::~Application()
{
}

bool Application::createWindow(const char * title, int width, int height, bool fullscreen)
{
	if (glfwInit() == GL_FALSE)
		return false;

	m_window = glfwCreateWindow(width, height, title, (fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);

	if (m_window == nullptr) {
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}

	aie::Gizmos::create(65535U, 65535U, 255U, 255U);

	m_view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.f);


	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glEnable(GL_DEPTH_TEST);

	return true;
}

void Application::destroyWindow()
{
	aie::Gizmos::destroy();

	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Application::run(const char * title, int width, int height, bool fullscreen)
{
	if(createWindow(title,width,height, fullscreen) && startup())
	{
		double prevTime = glfwGetTime();
		double currTime = 0;
		double deltaTime = 0;


		while (!m_gameOver)
		{
			currTime = glfwGetTime();
			deltaTime = currTime - prevTime;
			prevTime = currTime;
			
			glfwPollEvents();

			update(float(deltaTime));
			draw();

			glfwSwapBuffers(m_window);

			m_gameOver = m_gameOver || hasWindowClosed();
		}
	}

	shutdown();
	destroyWindow();
}

void Application::clearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Application::setBackgroundColour(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

bool Application::hasWindowClosed() const
{
	return glfwWindowShouldClose(m_window) == GLFW_TRUE;
}

unsigned int Application::getWindowWidth() const
{
	int w = 0, h = 0;
	glfwGetWindowSize(m_window, &w, &h);
	return w;
}

unsigned int Application::getWindowHeight() const
{
	int w = 0, h = 0;
	glfwGetWindowSize(m_window, &w, &h);
	return h;
}
