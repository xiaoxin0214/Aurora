#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "Events/Event.h"
#include "Input.h"
#include "GLFW/glfw3.h"
#include "Core/Timestep.h"
#include "Renderer/Renderer.h"
namespace Aurora
{
	Application* Application::s_pInstance = NULL;
	Application::Application()
	{
		AURORA_CORE_ASSERT(NULL == s_pInstance, "Application 已经初始化过了！");
		s_pInstance = this;
		m_pWindow = Scope<Window>(Window::Create());
		m_pWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_isRunning = true;
		m_lastFrameTime = 0.0f;
		m_pImguiLayer = new ImGuiLayer();
		Renderer::Init();
		PushOverlay(m_pImguiLayer);
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(const Event& e)
	{
		EventDispatcher dispatcher(const_cast<Event&>(e));
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowCloseEvent));

		for (auto iter = m_lyrStack.end(); iter != m_lyrStack.begin();)
		{
			(*--iter)->OnEvent(e);
			if (e.IsHandled())
				break;
		}
	}

	void Application::PushLayer(Layer* lyr)
	{
		m_lyrStack.PushLayer(lyr);
		lyr->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_lyrStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowCloseEvent(const WindowCloseEvent& e)
	{
		m_isRunning = false;
		return true;
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			float time = (float)glfwGetTime();
			Timestep timestep(time-m_lastFrameTime);
			m_lastFrameTime = time;
			for (Layer* lyr : m_lyrStack)
			{
				lyr->OnUpdate(timestep);
			}

			m_pImguiLayer->Begin();
			for (Layer* lyr : m_lyrStack)
			{
				lyr->OnImGuiRender();
			}
			m_pImguiLayer->End();

			m_pWindow->OnUpdate();
		}
	}

}