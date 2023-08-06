#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "Events/Event.h"
#include "glad/glad.h"
#include "Input.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
namespace Aurora
{
	Application* Application::s_pInstance = NULL;
	Application::Application()
	{
		AURORA_CORE_ASSERT(NULL == s_pInstance, "Application 已经初始化过了！");
		s_pInstance = this;
		m_pWindow = std::unique_ptr<Window>(Window::Create());
		m_pWindow->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
		m_isRunning = true;
		m_pImguiLayer = new ImGuiLayer();
		PushOverlay(m_pImguiLayer);

		glGenVertexArrays(1, &m_vertexArray);
		glBindVertexArray(m_vertexArray);


		float vertices[3 * 3] = {
			-0.5f,-0.5f,0.0f,
			0.5f,-0.5f,0.0f,
			0.0f,0.5f,0.0f
		};

		unsigned int indices[3] = {
			0,1,2
		};

		m_vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);


		std::string vs = R"(
				#version 330 core
				layout(location = 0) in vec3 a_pos;
				out vec3 v_pos;
				void main()
				{
					v_pos=a_pos;
					gl_Position=vec4(a_pos,1.0);
				}
			)";
		std::string fs = R"(
				#version 330 core
				layout(location = 0) out vec4 color;
				in vec3 v_pos;
				void main()
				{
					color=vec4(v_pos,1.0);
				}
			)";
		m_shader.reset(new OpenGLShader(vs, fs));
		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();
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
			glClear(GL_COLOR_BUFFER_BIT);
			m_shader->Bind();
			glBindVertexArray(m_vertexArray);
			glDrawElements(GL_TRIANGLES, m_indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);
			for (Layer* lyr : m_lyrStack)
			{
				lyr->OnUpdate();
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