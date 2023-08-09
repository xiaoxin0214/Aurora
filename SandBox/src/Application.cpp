
#include "Aurora.h"

namespace Aurora
{
	class ExampleLayer :public Aurora::Layer {
	public:
		ExampleLayer() :Layer("Example"), m_camera(-1.6f, 1.6f, -0.9f, 0.9f),m_cameraPosition(glm::vec3(0.0))
		{
			float vertices[3 * 7] = {
		-0.5f,-0.5f,0.0f,1.0f,0.0f,0.0f,1.0f,
		0.5f,-0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,
		0.0f,0.5f,0.0f,0.0f,0.0f,1.0f,1.0f
			};

			BufferLayout layout({ {ShaderDataType::Float3,"a_pos"},{ShaderDataType::Float4,"a_color"} });

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
				uniform mat4 u_viewProjection;
				out vec3 v_pos;
				out vec4 v_color;
				void main()
				{
					v_pos=a_pos;
					v_color= a_color;
					gl_Position=u_viewProjection*vec4(a_pos,1.0);
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
			m_shader.reset(Shader::Create(vs, fs));
		}

		void OnUpdate()override {
			//AURORA_TRACE("ExampleLayer::OnUpdate");

			// 在updata中做而不是在事件中做，在事件中做移动不够丝滑
			if (Input::IsKeyPressed(AURORA_KEY_A))
			{
				m_cameraPosition.x += 0.1f;
			}
			else if (Input::IsKeyPressed(AURORA_KEY_W))
			{
				m_cameraPosition.y -= 0.1f;
			}
			else if (Input::IsKeyPressed(AURORA_KEY_S))
			{
				m_cameraPosition.y += 0.1f;
			}
			else if (Input::IsKeyPressed(AURORA_KEY_D))
			{
				m_cameraPosition.x -= 0.1f;
			}

			m_camera.SetPosition(m_cameraPosition);

			RendererCommand::Clear();
			Renderer::BeginScene(m_camera);
			Renderer::Submit(m_vertexArray, m_shader);
			Renderer::EndScene();
		}

		void OnEvent(const Aurora::Event& e)override
		{
			EventDispatcher dispatcher(const_cast<Event&>(e));
		}

	private:
		bool OnKeyPressedEvent(KeyPressedEvent&e)
		{
			return false;
		}
	private:
		std::shared_ptr<VertexArray> m_vertexArray;
		std::shared_ptr<Shader> m_shader;
		OrthographicCamera      m_camera;
		glm::vec3               m_cameraPosition;
	};
}

class SandBox :public Aurora::Application {
public:
	SandBox()
	{
		PushLayer(new Aurora::ExampleLayer());
	}
};


Aurora::Application* Aurora::CreateApplication()
{
	return new SandBox();
}