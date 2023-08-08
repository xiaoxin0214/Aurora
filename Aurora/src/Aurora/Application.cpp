#include "pch.h"
#include "Application.h"
#include "Log.h"
#include "Events/Event.h"
#include "Input.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/BufferLayout.h"
#include "Renderer/Renderer.h"
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

		float vertices[3 * 7] = {
			-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,
			0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,
			0.0f,0.5f,0.0f,0.0f,0.0f,1.0f,1.0f
		};

		BufferLayout layout({ {ShaderDataType::Float3,"a_pos"},{ShaderDataType::Float4,"a_color"}});

		unsigned int indices[3] = {
			0,1,2
		};

		std::shared_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));
		vertexBuffer->SetLayout(layout);

		m_vertexArray.reset(VertexArray::Create());
		m_vertexArray->AddVertexBuffer(vertexBuffer);
		m_vertexArray->SetIndexBuffer(indexBuffer);

		std::string vs = R"(
				#version 330 core
				layout(location = 0) in vec3 a_pos;
				layout(location =1) in vec4 a_color;
				out vec3 v_pos;
				out vec4 v_color;
				void main()
				{
					v_pos=a_pos;
					v_color= a_color;
					gl_Position=vec4(a_pos,1.0);
				}
			)";
		std::string fs = R"(
				#version 330 core
				layout(location = 0) out vec4 color;
				in vec3 v_pos;
				in vec4 v_color;
				void main()
				{
					color=v_color;
				}
			)";
		m_shader.reset(new OpenGLShader(vs, fs));
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
			m_shader->Bind();
			Renderer::BeginScene();
			Renderer::Submit(m_vertexArray);
			Renderer::EndScene();

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