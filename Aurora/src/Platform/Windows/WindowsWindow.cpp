#include "pch.h"
#include "WindowsWindow.h"
#include "Aurora/Core/Log.h"
#include "Aurora/Events/ApplicationEvent.h"
#include "Aurora/Events/KeyEvent.h"
#include "Aurora/Events/MouseEvent.h"
#include "GLFW/glfw3.h"
#include "Platform/OpenGL/OpenGLContext.h"

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

	static void GLFWErrorCallback(int error, const char* description)
	{
		AURORA_CORE_ERROR("GLFW ERROR ({0}):{1}", error, description);
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_data.title = props.title;
		m_data.width = props.width;
		m_data.height = props.height;

		AURORA_CORE_INFO("正在创建窗口{0},({1},{2})", m_data.title, m_data.width, m_data.height);

		if (!s_glfwInited)
		{
			int success = glfwInit();
			AURORA_CORE_ASSERT(success, "GLFW 初始化失败!")
				s_glfwInited = true;
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		m_pWindow = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), NULL, NULL);
		m_pContext = new OpenGLContext(m_pWindow);
		m_pContext->Init();

		glfwSetWindowUserPointer(m_pWindow, &m_data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_pWindow, [](GLFWwindow* pWindow, int width, int height) ->void {
			WindowData* pData = (WindowData*)glfwGetWindowUserPointer(pWindow);
			pData->width = width;
			pData->height = height;
			pData->callback(WindowResizeEvent(width, height));
			});

		glfwSetWindowCloseCallback(m_pWindow, [](GLFWwindow* pWindow)->void {
			WindowData* pData = (WindowData*)glfwGetWindowUserPointer(pWindow);
			pData->callback(WindowCloseEvent());
			});

		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* pWindow, int key, int scancode, int action, int mods)->void {
			WindowData* pData = (WindowData*)glfwGetWindowUserPointer(pWindow);
			switch (action)
			{
			case GLFW_PRESS:
				pData->callback(KeyPressedEvent(key, 0));
				break;
			case GLFW_RELEASE:
				pData->callback(KeyReleasedEvent(key));
				break;
			case GLFW_REPEAT:
				pData->callback(KeyPressedEvent(key, 1));
				break;
			default:
				break;
			}
			});

		glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* pWindow, int button, int action, int mods)->void {
			WindowData* pData = (WindowData*)glfwGetWindowUserPointer(pWindow);
			switch (action)
			{
			case GLFW_PRESS:
			{
				pData->callback(MouseButtonPressedEvent(button));
				break;
			}
			case GLFW_RELEASE:
			{
				pData->callback(MouseButtonReleasedEvent(button));
				break;
			}
			}
			});

		glfwSetScrollCallback(m_pWindow, [](GLFWwindow* pWindow, double xoffset, double yoffset)->void {
			WindowData* pData = (WindowData*)glfwGetWindowUserPointer(pWindow);
			pData->callback(MouseScrolledEvent((float)xoffset, (float)yoffset));
			});

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* pWindow, double xpos, double ypos)->void {
			WindowData* pData = (WindowData*)glfwGetWindowUserPointer(pWindow);
			pData->callback(MouseMovedEvent((float)xpos, (float)ypos));
			});

		glfwSetCharCallback(m_pWindow, [](GLFWwindow* pWindow, unsigned int codepoint)->void {
			WindowData* pData = (WindowData*)glfwGetWindowUserPointer(pWindow);
			pData->callback(KeyTypedEvent(codepoint));
			});
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
		m_pContext->SwapBuffers();
	}
}