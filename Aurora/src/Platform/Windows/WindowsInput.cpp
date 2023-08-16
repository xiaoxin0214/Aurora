#include "pch.h"
#include "WindowsInput.h"
#include "Aurora/Core/Application.h"
#include "GLFW/glfw3.h"
namespace Aurora
{
	Input* Input::s_pInstance = new WindowsInput();
	bool WindowsInput::IsKeyPressedImpl(int keyCode)
	{
		auto pWindow = static_cast<GLFWwindow*>(Application::GetInstance()->GetNativeWindow());
		int state = glfwGetKey(pWindow, keyCode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(int button)
	{
		auto pWindow = static_cast<GLFWwindow*>(Application::GetInstance()->GetNativeWindow());
		int state = glfwGetMouseButton(pWindow, button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return x;
	}

	float WindowsInput::GetMouseYImpl()
	{
		auto [x, y] = GetMousePosImpl();
		return y;
	}

	std::pair<float, float> WindowsInput::GetMousePosImpl()
	{
		double x, y;
		auto pWindow = static_cast<GLFWwindow*>(Application::GetInstance()->GetNativeWindow());
		glfwGetCursorPos(pWindow, &x, &y);
		return std::make_pair((float)x, (float)y);
	}
}