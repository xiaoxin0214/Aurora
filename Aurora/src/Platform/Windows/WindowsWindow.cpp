#include "pch.h"
#include "WindowsWindow.h"
#include "Aurora/Log.h"
namespace Aurora
{

	static bool s_glfwInited = false;
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		ShutDown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		AURORA_CORE_INFO("正在创建窗口{0},({1},{2})",m_data.title,m_data.width,m_data.height);

		if (!s_glfwInited)
		{
			int success = glfwInit();
			AURORA_CORE_ASSERT(success,"GLFW 初始化失败!")
			s_glfwInited = true;
		}

		m_pWindow = glfwCreateWindow(m_data.width,m_data.height,m_data.title.c_str(),NULL,NULL);
		glfwMakeContextCurrent(m_pWindow);
		glfwSetWindowUserPointer(m_pWindow,&m_data);
		SetVSync(true);
	}
	
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}

		m_data.vSync = enabled;
	}

	void WindowsWindow::ShutDown()
	{
		glfwDestroyWindow(m_pWindow);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
}