
#include "Aurora.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "imgui.h"
namespace Aurora
{
	class ExampleLayer :public Aurora::Layer {
	public:
		ExampleLayer() :Layer("Example"), m_cameraController(1960.0f / 1080.f)
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

			m_shader = m_shaderLibrary.LoadShader("asset\\shaders\\test.glsl");
			m_texture = Texture2D::Create("asset\\textures\\test.jpeg");
			m_transparentTexture = Texture2D::Create("asset\\textures\\transparent.png");
		}

		void OnUpdate(Timestep& timestep)override {
			//AURORA_TRACE("ExampleLayer::OnUpdate");

			float ts = timestep;
			m_cameraController.OnUpdate(timestep);

			m_shader->Bind();
			m_texture->Bind(0);
			m_shader->SetUniformFloat3("u_color", m_color);
			m_shader->SetUniformInt("u_texture", 0);

			RendererCommand::Clear();
			Renderer::BeginScene(m_cameraController.GetCamera());
			glm::vec3 position(0.0f);
			glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0), position);
			Renderer::Submit(m_vertexArray, m_shader, modelMatrix);

			m_transparentTexture->Bind(0);
			m_shader->SetUniformInt("u_texture", 0);
			Renderer::Submit(m_vertexArray, m_shader, modelMatrix);

			Renderer::EndScene();
		}

		void OnEvent(const Aurora::Event& e)override
		{
			EventDispatcher dispatcher(const_cast<Event&>(e));
			m_cameraController.OnEvent(e);
		}

		void OnImGuiRender()override
		{
			ImGui::Begin("…Ë÷√");
			ImGui::ColorEdit3("—’…´", glm::value_ptr(m_color));
			ImGui::End();
		}

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e)
		{
			return false;
		}
	private:
		Ref<VertexArray>                  m_vertexArray;
		Ref<Shader>                       m_shader;
		ShaderLibrary                     m_shaderLibrary;
		Ref<Texture2D>                    m_texture;
		Ref<Texture2D>                    m_transparentTexture;
		OrthographicCameraController      m_cameraController;
		glm::vec3                         m_color;
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