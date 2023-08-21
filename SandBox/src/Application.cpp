
#include "Aurora.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "imgui.h"
namespace Aurora
{
	class ExampleLayer :public Aurora::Layer {
	public:
		ExampleLayer() :Layer("Example"), m_cameraController(1960.0f / 1080.f),m_color(glm::vec4(1.0f,0.0f,0.0f,1.0f))
		{
			m_texture = Texture2D::Create("asset\\textures\\transparent.png");
		}

		void OnUpdate(Timestep& timestep)override {
			//AURORA_TRACE("ExampleLayer::OnUpdate");

			float ts = timestep;
			m_cameraController.OnUpdate(timestep);

			RendererCommand::Clear();
			Renderer2D::BeginScene(m_cameraController.GetCamera());

			Renderer2D::DrawQuad(glm::vec2(0.5f,0.0f),glm::vec2(0.5f,0.5f),0.0f, m_color);
			Renderer2D::DrawQuad(glm::vec2(-0.5f, 0.0f), glm::vec2(0.5f, 0.5f), 0.0f, glm::vec4(1.0f,1.0f,0.0f,1.0f));

			Renderer2D::DrawQuad(glm::vec2(-0.5f, 0.0f), glm::vec2(1.0f),0.0, m_texture,glm::vec4(1.0f,1.0f,1.0f,1.0f));
			Renderer2D::EndScene();
		}

		void OnEvent(const Aurora::Event& e)override
		{
			EventDispatcher dispatcher(const_cast<Event&>(e));
			m_cameraController.OnEvent(e);
		}

		void OnImGuiRender()override
		{
			ImGui::Begin("Settings");
			ImGui::ColorEdit4("ÑÕÉ«", glm::value_ptr(m_color));
			ImGui::End();
		}

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e)
		{
			return false;
		}
	private:
		OrthographicCameraController      m_cameraController;
		Ref<Texture>                      m_texture;
		glm::vec4                         m_color;
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