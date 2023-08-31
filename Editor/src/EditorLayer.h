#pragma once
#include "Aurora.h"
namespace Aurora
{
	class EditorLayer :public Layer {
	public:
		EditorLayer();

		void OnAttach()override;

		void OnUpdate(Timestep& timestep)override;

		void OnEvent(const Aurora::Event& e)override;

		void OnImGuiRender()override;

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e)
		{
			return false;
		}
	private:
		OrthographicCameraController      m_cameraController;
		Ref<Texture>                      m_texture;
		Ref<FrameBuffer>                  m_frameBuffer;
		glm::vec4                         m_color;
		glm::vec2                         m_viewportSize;
	};
}