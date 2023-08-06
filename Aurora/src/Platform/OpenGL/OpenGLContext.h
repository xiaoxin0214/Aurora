#pragma once
#include "Aurora/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Aurora
{
	class AURORA_API OpenGLContext :public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* pWindow);
	public:
		void Init()override;
		void SwapBuffers()override;
	private:
		GLFWwindow* m_pWindow;
	};
}