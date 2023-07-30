#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "Events/Event.h"
namespace Aurora
{
#define BIND_EVENT_FN(func) std::bind(&func,this,std::placeholders::_1)
	Application::Application()
	{
		m_pWindow = std::unique_ptr<Window>(Window::Create());
		m_pWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_isRunning = true;
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
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_lyrStack.PushOverlay(overlay);
	}

	bool Application::OnWindowCloseEvent(const WindowCloseEvent&e)
	{
		m_isRunning = false;
		return true;
	}

	void Application::Run()
	{
		while (m_isRunning)
		{
			m_pWindow->OnUpdate();
			for (Layer* lyr : m_lyrStack)
			{
				lyr->OnUpdate();
			}
		}
	}

}