#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
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
		void PushLayer(Layer* lyr);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_pWindow;
		bool                    m_isRunning;
		LayerStack              m_lyrStack;
	};

	Application* CreateApplication();
}
