#pragma once
#include "Core.h"
#include "Window.h"
#include "Aurora/Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "Aurora/ImGui/ImGuiLayer.h"
namespace Aurora
{
	class AURORA_API Application
	{
	public:
		Application(const std::string&name);
		~Application();
	public:
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* lyr);
		void PushOverlay(Layer* overlay);
		inline Window* GetWindow()
		{
			return m_pWindow.get();
		}

		inline void* GetNativeWindow()const {
			return m_pWindow->GetNativeWindow();
		}

		inline ImGuiLayer* GetImGuiLayer()
		{
			return m_pImguiLayer;
		}

		static Application* GetInstance()
		{
			return s_pInstance;
		}
	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
		bool OnWindowResizeEvent(const WindowResizeEvent&e);
	private:
		static Application*              s_pInstance;
		Scope<Window>                    m_pWindow;
		ImGuiLayer*                      m_pImguiLayer;
		bool                             m_isRunning;
		bool                             m_minimized;
		LayerStack                       m_lyrStack;
		float                            m_lastFrameTime;

	};	

	Application* CreateApplication();
}
