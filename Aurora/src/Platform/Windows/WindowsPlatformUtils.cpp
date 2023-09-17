#include "pch.h"
#include "Aurora/Utils/PlatformUtils.h"
#include "Aurora/Core/Application.h"

#include <commdlg.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

namespace Aurora
{
	std::string FileDialog::OpenFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[256]{0};
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		GLFWwindow*pNativeWindow=(GLFWwindow*)Application::GetInstance()->GetNativeWindow();
		ofn.hwndOwner = glfwGetWin32Window(pNativeWindow);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}

	std::string FileDialog::SaveFile(const char* filter)
	{
		OPENFILENAMEA ofn;
		CHAR szFile[256]{ 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);
		GLFWwindow* pNativeWindow = (GLFWwindow*)Application::GetInstance()->GetNativeWindow();
		ofn.hwndOwner = glfwGetWin32Window(pNativeWindow);
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}

		return std::string();
	}
}