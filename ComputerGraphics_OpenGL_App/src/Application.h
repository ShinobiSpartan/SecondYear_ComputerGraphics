#pragma once
#include <glm/ext.hpp>

struct GLFWwindow;

class Application
{
public:
	Application();
	virtual ~Application();

	void run(const char* title, int width, int height, bool fullscreen);

	virtual bool startup() = 0;
	virtual void shutdown() = 0;
	virtual void update(float deltaTime) = 0;
	virtual void draw() = 0;

	void clearScreen();

	void setBackgroundColour(float r, float g, float b, float a = 1.0f);

	void quit() { m_gameOver = true; }

	bool hasWindowClosed() const;

	unsigned int getWindowWidth() const;
	unsigned int getWindowHeight() const;

protected:
	virtual bool	createWindow(const char* title, int width, int height, bool fullscreen);
	virtual void	destroyWindow();

	GLFWwindow*		m_window;

	glm::mat4		m_view;
	glm::mat4		m_projection;

	bool			m_gameOver;

};

