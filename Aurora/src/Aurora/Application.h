#pragma once
#include "Core.h"
#include "Window.h"
#include "Events/ApplicationEvent.h"
#include "LayerStack.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/VertexArray.h"
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
		inline Window* GetWindow()
		{
			return m_pWindow.get();
		}

		inline void* GetNativeWindow()const {
			return m_pWindow->GetNativeWindow();
		}

		static Application* GetInstance()
		{
			return s_pInstance;
		}
	private:
		bool OnWindowCloseEvent(const WindowCloseEvent& e);
	private:
		static Application*              s_pInstance;
		std::unique_ptr<Window>          m_pWindow;
		ImGuiLayer*                      m_pImguiLayer;
		bool                             m_isRunning;
		LayerStack                       m_lyrStack;
	private:
		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<Shader> m_shader;
	};	

	Application* CreateApplication();
}
