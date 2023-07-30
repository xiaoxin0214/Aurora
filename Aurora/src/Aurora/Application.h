#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
namespace Aurora
{
	class AURORA_API Application
	{
	public:
		Application();
		~Application();
	public:
		void Run();
		void OnEvent(const Event& e);
	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_pWindow;
		bool                    m_isRunning;
	};

	Application* CreateApplication();
}
