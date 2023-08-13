
#include "Aurora.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "imgui.h"
namespace Aurora
{
	class ExampleLayer :public Aurora::Layer {
	public:
		ExampleLayer() :Layer("Example"), m_camera(-1.0f, 1.0f, -1.0f, 1.0f), m_cameraPosition(glm::vec3(0.0)), m_cameraMoveSpeed(0.1f)
		{
			m_color.x = 1.0f;

			float vertices[4 * 5] = {
		-1.0f,-1.0f,0.0f,0.0f,0.0f,
		1.0f,-1.0f,0.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,1.0f,1.0f,
		-1.0f,1.0f,0.0f,0.0f,1.0f

			};

			BufferLayout layout({
				{ShaderDataType::Float3,"a_pos"},
				{ShaderDataType::Float2,"a_texcoord"}
				});

			unsigned int indices[6] = {
				0,1,2,2,3,0
			};

			Ref<IndexBuffer> indexBuffer;
			Ref<VertexBuffer> vertexBuffer;
			vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
			indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)));
			vertexBuffer->SetLayout(layout);

			m_vertexArray.reset(VertexArray::Create());
			m_vertexArray->AddVertexBuffer(vertexBuffer);
			m_vertexArray->SetIndexBuffer(indexBuffer);

			std::string vs = R"(
				#version 330 core
				layout(location = 0) in vec3 a_pos;
				layout(location =1)  in vec2 a_texcoord;
				uniform mat4 u_viewProjection;
				uniform mat4 u_modelMatrix;
				out vec3 v_pos;
				out vec2 v_texcoord;
				void main()
				{
					v_pos=a_pos;
					v_texcoord=a_texcoord;
					gl_Position=u_modelMatrix*u_viewProjection*vec4(a_pos,1.0);
				}
			)";
			std::string fs = R"(
				#version 330 core
				layout(location = 0) out vec4 color;
				uniform vec3 u_color;
				uniform sampler2D u_texture;
				in vec3 v_pos;
				in vec2 v_texcoord;
				void main()
				{
					color=texture(u_texture,v_texcoord);
					//color*=vec4(u_color,1.0);
				}
			)";
			m_shader.reset(Shader::Create(vs, fs));
			m_texture = Texture2D::Create("asset\\textures\\test.jpeg");
		}

		void OnUpdate(Timestep& timestep)override {
			//AURORA_TRACE("ExampleLayer::OnUpdate");

			float ts = timestep;
			AURORA_CORE_TRACE("timestep:{0}", timestep.GetMilliSeconds());
			// 在updata中做而不是在事件中做，在事件中做移动不够丝滑
			if (Input::IsKeyPressed(AURORA_KEY_A))
			{
				m_cameraPosition.x += m_cameraMoveSpeed * ts;
			}
			else if (Input::IsKeyPressed(AURORA_KEY_W))
			{
				m_cameraPosition.y -= m_cameraMoveSpeed * ts;
			}
			else if (Input::IsKeyPressed(AURORA_KEY_S))
			{
				m_cameraPosition.y += m_cameraMoveSpeed * ts;
			}
			else if (Input::IsKeyPressed(AURORA_KEY_D))
			{
				m_cameraPosition.x -= m_cameraMoveSpeed * ts;
			}

			m_camera.SetPosition(m_cameraPosition);

			m_shader->Bind();
			m_texture->Bind(0);
			m_shader->SetUniformFloat3("u_color", m_color);
			m_shader->SetUniformInt("u_texture", 0);

			RendererCommand::Clear();
			Renderer::BeginScene(m_camera);
			glm::vec3 position(0.0f);
			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), position);
			Renderer::Submit(m_vertexArray, m_shader, modelMatrix);
			Renderer::EndScene();
		}

		void OnEvent(const Aurora::Event& e)override
		{
			EventDispatcher dispatcher(const_cast<Event&>(e));
		}

		void OnImGuiRender()override
		{
			ImGui::Begin("设置");
			ImGui::ColorEdit3("颜色", glm::value_ptr(m_color));
			ImGui::End();
		}

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e)
		{
			return false;
		}
	private:
		Ref<VertexArray> m_vertexArray;
		Ref<Shader> m_shader;
		Ref<Texture2D>          m_texture;
		OrthographicCamera      m_camera;
		glm::vec3               m_color;
		glm::vec3               m_cameraPosition;
		float                   m_cameraMoveSpeed;
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