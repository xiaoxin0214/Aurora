#include "EditorLayer.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
namespace Aurora
{
	EditorLayer::EditorLayer() :Layer("EditorLayer"), m_viewportSize(0.0, 0.0), m_cameraController(1960.0f / 1080.f), m_color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)), m_viewportFocused(false), m_viewportHovered(false)
	{
		m_texture = Texture2D::Create("asset\\textures\\tilemap_packed.png");
	}

	void EditorLayer::OnAttach()
	{
		FrameBufferProps props;
		props.width = 1960;
		props.height = 1080;
		props.samples = 1;
		m_frameBuffer = FrameBuffer::Create(props);
		m_scene = CreateRef<Scene>();
		auto square = m_scene->CreateEntity("square");
		square.AddComponent<MeshComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_cameraEntity = m_scene->CreateEntity("camera");
		m_cameraEntity.AddComponent<CameraComponent>();

		class CameraController :public ScriptableEntity
		{
		private:
			float              m_cameraMoveSpeed;
		public:
			void OnCreate() override
			{
				m_cameraMoveSpeed = 5.0f;
				AURORA_INFO("CameraController OnCreate");
			}

			void OnDestroy() override
			{
			}

			void OnUpdate(Timestep ts) override
			{
				auto& transformComponent = GetComponent<TransformComponent>();
				// ��updata�������������¼����������¼������ƶ�����˿��
				if (Input::IsKeyPressed(AURORA_KEY_A))
				{

					transformComponent.position.x += m_cameraMoveSpeed * ts;
				}
				else if (Input::IsKeyPressed(AURORA_KEY_W))
				{
					transformComponent.position.y -= m_cameraMoveSpeed * ts;
				}
				else if (Input::IsKeyPressed(AURORA_KEY_S))
				{
					transformComponent.position.y += m_cameraMoveSpeed * ts;
				}
				else if (Input::IsKeyPressed(AURORA_KEY_D))
				{
					transformComponent.position.x -= m_cameraMoveSpeed * ts;
				}
			}
		};

		m_cameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_sceneHierarchyPanel.SetContext(m_scene);
	}

	void EditorLayer::OnUpdate(Timestep& timestep)
	{
		float ts = timestep;
		if (m_viewportFocused)
		{
			m_cameraController.OnUpdate(timestep);
		}

		Renderer2D::ResetStatistics();
		m_frameBuffer->Bind();
		RendererCommand::Clear();

		m_scene->OnUpdate(ts);

		m_frameBuffer->UnBind();
	}

	void EditorLayer::OnEvent(Aurora::Event& e)
	{
		EventDispatcher dispatcher(e);
		m_cameraController.OnEvent(e);
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool p_open = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);

		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		m_sceneHierarchyPanel.OnImGuiRender();

		auto stats = Renderer2D::GetStatistics();

		ImGui::Begin("stats");
		ImGui::Text(u8"������Ϣ:");
		ImGui::Text(u8"��Ⱦ����:%d", stats.drawCalls);
		ImGui::Text(u8"���θ���:%d", stats.quadCount);

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("viewport");
		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();

		Application::GetInstance()->GetImGuiLayer()->BlockEvents(!m_viewportFocused || !m_viewportHovered);

		auto region = ImGui::GetContentRegionAvail();
		if (m_viewportSize.x != region.x || m_viewportSize.y != region.y)
		{
			m_frameBuffer->Resize((std::uint32_t)region.x, (std::uint32_t)region.y);
			m_viewportSize.x = region.x;
			m_viewportSize.y = region.y;
			m_cameraController.OnResize(m_viewportSize.x, m_viewportSize.y);
			m_scene->OnViewportResize(m_viewportSize.x, m_viewportSize.y);
		}
		auto textureID = m_frameBuffer->GetColorAttachmentID();
		ImGui::Image((void*)textureID, ImVec2(m_viewportSize.x, m_viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}
}