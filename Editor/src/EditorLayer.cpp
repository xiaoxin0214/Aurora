#include "EditorLayer.h"
#include "imgui.h"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "ImGuizmo.h"

namespace Aurora
{
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
			// 在updata中做而不是在事件中做，在事件中做移动不够丝滑
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

	EditorLayer::EditorLayer() :Layer("EditorLayer"), m_viewportSize(0.0, 0.0), m_color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)), m_viewportFocused(false), m_viewportHovered(false), m_imguizmoOperation(-1), m_contentBrowserPanel("asset")
	{
		m_texture = Texture2D::Create("asset\\textures\\tilemap_packed.png");
	}

	void EditorLayer::OnAttach()
	{
		FrameBufferProps props;
		props.width = 1960;
		props.height = 1080;
		props.samples = 1;
		props.attachments = { FrameBufferTextureFormat::RGBA8,FrameBufferTextureFormat::REDINTEGER,FrameBufferTextureFormat::DEPTH };
		m_frameBuffer = FrameBuffer::Create(props);
		m_scene = CreateRef<Scene>();

		m_sceneHierarchyPanel.SetContext(m_scene);

	}

	void EditorLayer::OnUpdate(Timestep& timestep)
	{
		float ts = timestep;
		if (m_viewportFocused)
		{
			m_editorCamera.OnUpdate(ts);
		}

		Renderer2D::ResetStatistics();
		m_frameBuffer->Bind();
		RendererCommand::Clear();
		m_frameBuffer->ClearColorAttachment(1, -1);

		m_scene->OnUpdateEditor(ts, m_editorCamera);

		auto mousePos = ImGui::GetMousePos();
		if (m_viewportBounds.Contains(mousePos.x, mousePos.y))
		{
			std::uint32_t x = mousePos.x - m_viewportBounds.xmin;
			std::uint32_t y = m_viewportBounds.Height() - (mousePos.y - m_viewportBounds.ymin);
			int pixelData=m_frameBuffer->ReadPixel(1,x,y);
			m_hoveredEntity = pixelData == -1 ? Entity() : Entity({(entt::entity)pixelData,m_scene.get()});
		}

		m_frameBuffer->UnBind();
	}

	bool EditorLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool isCtrlPressed = Input::IsKeyPressed(AURORA_KEY_LEFT_CONTROL) || Input::IsKeyPressed(AURORA_KEY_RIGHT_CONTROL);
		bool isShiftPressed = Input::IsKeyPressed(AURORA_KEY_LEFT_SHIFT) || Input::IsKeyPressed(AURORA_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{
		case AURORA_KEY_N:
		{
			if (isCtrlPressed)
			{
				NewScene();
			}
			break;
		}
		case AURORA_KEY_O:
		{
			if (isCtrlPressed)
			{
				OpenScene();
			}
			break;
		}
		case AURORA_KEY_S:
		{
			if (isCtrlPressed && isShiftPressed)
			{
				SaveAsScene();
			}
			break;
		}

		case AURORA_KEY_Q:
		{
			m_imguizmoOperation = -1;
			break;
		}
		case AURORA_KEY_W:
		{
			m_imguizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case AURORA_KEY_E:
		{
			m_imguizmoOperation = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case AURORA_KEY_R:
		{
			m_imguizmoOperation = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}
		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (AURORA_MOUSE_BUTTON_LEFT == e.GetButton())
		{
			if (m_hoveredEntity&&!ImGuizmo::IsOver())
			{
				m_sceneHierarchyPanel.SetSelectedEntity(m_hoveredEntity);
			}
		}
		return false;
	}

	void EditorLayer::OnEvent(Aurora::Event& e)
	{
		m_editorCamera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		dispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	void EditorLayer::NewScene()
	{
		m_scene = CreateRef<Scene>();
		m_scene->OnViewportResize(m_viewportSize.x, m_viewportSize.y);
		m_sceneHierarchyPanel.SetContext(m_scene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filePath = FileDialog::OpenFile("Scene(*.scene)\0*.scene\0");
		if (!filePath.empty())
		{
			OpenScene(filePath);
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		NewScene();
		SceneSerializer sceneSerializer(m_scene);
		sceneSerializer.Deserialize(path.string());
	}

	void EditorLayer::SaveAsScene()
	{
		std::string filePath = FileDialog::SaveFile("Scene(*.scene)\0*.scene\0");
		if (!filePath.empty())
		{
			SceneSerializer sceneSerializer(m_scene);
			sceneSerializer.Serialize(filePath);
		}
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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu(u8"文件"))
			{
				if (ImGui::MenuItem(u8"新建", u8"Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem(u8"打开", u8"Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem(u8"另存为", "Ctrl+Shift+S"))
				{
					SaveAsScene();
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		m_sceneHierarchyPanel.OnImGuiRender();
		m_contentBrowserPanel.OnImGuiRender();

		auto stats = Renderer2D::GetStatistics();

		ImGui::Begin("stats");
		ImGui::Text(u8"绘制信息:");
		ImGui::Text(u8"渲染批次:%d", stats.drawCalls);
		ImGui::Text(u8"矩形个数:%d", stats.quadCount);

		ImGui::End();


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("viewport");
		m_viewportFocused = ImGui::IsWindowFocused();
		m_viewportHovered = ImGui::IsWindowHovered();

		auto viewportOffset = ImGui::GetCursorPos();

		Application::GetInstance()->GetImGuiLayer()->BlockEvents(!m_viewportFocused && !m_viewportHovered);

		auto region = ImGui::GetContentRegionAvail();
		if (m_viewportSize.x != region.x || m_viewportSize.y != region.y)
		{
			m_frameBuffer->Resize((std::uint32_t)region.x, (std::uint32_t)region.y);
			m_viewportSize.x = region.x;
			m_viewportSize.y = region.y;
			m_editorCamera.SetViewportSize(m_viewportSize.x, m_viewportSize.y);
			m_scene->OnViewportResize(m_viewportSize.x, m_viewportSize.y);
		}
		auto textureID = m_frameBuffer->GetColorAttachmentID(0);
		ImGui::Image((void*)textureID, ImVec2(m_viewportSize.x, m_viewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(path);
			}
			ImGui::EndDragDropTarget();
		}

		auto windowSize = ImGui::GetWindowSize();
		auto minBound = ImGui::GetWindowPos();

		m_viewportBounds.Set(minBound.x+viewportOffset.x, minBound.y+viewportOffset.y, minBound.x + windowSize.x, minBound.y + windowSize.y);


		Entity selectedEntity = m_sceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_imguizmoOperation != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			const glm::mat4& cameraProjectionMatrix = m_editorCamera.GetProjection();
			glm::mat4 cameraViewMatrix = m_editorCamera.GetViewMatrix();

			glm::mat4 selectedEntityTransform = selectedEntity.GetComponent<TransformComponent>();
			auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();

			ImGuizmo::Manipulate(glm::value_ptr(cameraViewMatrix), glm::value_ptr(cameraProjectionMatrix),
				(ImGuizmo::OPERATION)m_imguizmoOperation, ImGuizmo::MODE::LOCAL, glm::value_ptr(selectedEntityTransform));

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translate, rotation, scale;
				ImGuizmo::DecomposeMatrixToComponents(glm::value_ptr(selectedEntityTransform), glm::value_ptr(translate), glm::value_ptr(rotation), glm::value_ptr(scale));
				transformComponent.position = translate;
				transformComponent.scale = scale;
				transformComponent.rotation = rotation;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();

	}
}
