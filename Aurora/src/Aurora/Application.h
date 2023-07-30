#pragma once
#include "Core.h"
#include "Window.h"
namespace Aurora
{
	class AURORA_API Application
	{
	public:
		Application();
		~Application();
	public:
		void Run();
	private:
		std::unique_ptr<Window> m_pWindow;
		bool                    m_isRunning;
	};

	Application* CreateApplication();
}
