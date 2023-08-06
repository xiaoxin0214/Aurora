#include "pch.h"
#include "OpenGLContext.h"
#include "GLFW/glfw3.h"
#include "glad/glad.h"
#include "Aurora/Log.h"
namespace Aurora
{
	OpenGLContext::OpenGLContext(GLFWwindow* pWindow) :m_pWindow(pWindow) {
		AURORA_CORE_ASSERT(pWindow, "pWindow 入口参数如空!")
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_pWindow);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AURORA_CORE_ASSERT(status, "GLAD 初始化失败!");
		AURORA_CORE_INFO("OpenGL Info:");
		AURORA_CORE_INFO("    Vendor:{0}", (char*)glGetString(GL_VENDOR));
		AURORA_CORE_INFO("    Renderer:{0}", (char*)glGetString(GL_RENDERER));
		AURORA_CORE_INFO("    Version:{0}", (char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_pWindow);
	}
}