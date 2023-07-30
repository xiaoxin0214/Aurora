#include "pch.h"
#include "Application.h"
#include "GLFW/glfw3.h"
namespace Aurora
{
	Application::Application()
	{
		m_pWindow = std::unique_ptr<Window>(Window::Create());
		m_isRunning = true;
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		glClearColor(1.0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		while (m_isRunning)
		{
			m_pWindow->OnUpdate();
		}
	}

}