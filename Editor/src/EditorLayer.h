#pragma once
#include "Aurora.h"
#include "SceneHierarchyPanel.h"

namespace Aurora
{
	class EditorLayer :public Layer {
	public:
		EditorLayer();

		void OnAttach()override;

		void OnUpdate(Timestep& timestep)override;

		void OnEvent(Aurora::Event& e)override;

		void OnImGuiRender()override;

	private:
		bool OnKeyPressedEvent(KeyPressedEvent& e);

		void NewScene();
		void SaveAsScene();
		void OpenScene();
	private:
		Ref<Texture>                      m_texture;
		Ref<FrameBuffer>                  m_frameBuffer;
		Ref<Scene>                        m_scene;
		EditorCamera                      m_editorCamera;
		SceneHierarchyPanel               m_sceneHierarchyPanel;
		glm::vec4                         m_color;
		glm::vec2                         m_viewportSize;
		bool                              m_viewportFocused;
		bool                              m_viewportHovered;
		std::int32_t                      m_imguizmoOperation;
	};
}