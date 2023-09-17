#include "SceneHierarchyPanel.h"
#include "imgui.h"
#include "gtc/type_ptr.hpp"
namespace Aurora
{
	SceneHierarchyPanel::SceneHierarchyPanel(Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_context->m_registry.each(
			[&](auto entity) {
				DrawEntityNode({ entity,m_context.get() });
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selectedEntity = {};
		}

		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem(u8"创建Entity"))
			{
				m_context->CreateEntity("Entity");
			}

			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin(u8"属性");
		if (m_selectedEntity)
		{
			DrawComponents(m_selectedEntity);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;
		ImGuiTreeNodeFlags flags = (m_selectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0 | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(entity.GetID()), flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
		}

		bool removeEntity = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem(u8"删除Entity"))
			{
				removeEntity = true;
			}

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGui::TreePop();
		}

		if (removeEntity)
		{
			if (m_selectedEntity == entity)
			{
				m_selectedEntity = {};
			}
			m_context->DestroyEntity(entity);
		}
	}

	template<typename T, typename UIFunc>
	static void DrawComponent(const char* pszName, Entity entity, UIFunc func)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap;
		if (entity.HasComponent<T>())
		{
			bool removeComponent = false;
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, pszName);
			ImGui::SameLine(ImGui::GetWindowWidth() - 60);
			if (ImGui::Button("+", ImVec2(20, 20)))
			{
				ImGui::OpenPopup(u8"Component设置");
			}

			if (ImGui::BeginPopup(u8"Component设置"))
			{
				if (ImGui::MenuItem(u8"移除Component"))
				{
					removeComponent = true;
				}
				ImGui::EndPopup();
			}
			if (open)
			{

				func(entity.GetComponent<T>());
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, tag.c_str(), tag.size());
			if (ImGui::InputText(u8"##标签", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button(u8"添加component"))
		{
			ImGui::OpenPopup(u8"添加component");
		}

		if (ImGui::BeginPopup(u8"添加component"))
		{

			if (ImGui::MenuItem(u8"MeshComponent"))
			{
				m_selectedEntity.AddComponent<MeshComponent>(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem(u8"CameraComponent"))
			{
				m_selectedEntity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>(u8"变换", entity, [](TransformComponent& transformComponent) {
			ImGui::DragFloat3(u8"位置", glm::value_ptr(transformComponent.position), 0.1f);
			ImGui::DragFloat3(u8"缩放", glm::value_ptr(transformComponent.scale), 0.1f);
			ImGui::DragFloat3(u8"旋转", glm::value_ptr(transformComponent.rotation), 0.1f);
			});

		DrawComponent<CameraComponent>(u8"相机", entity, [](CameraComponent& cameraComponent) {
			auto& camera = cameraComponent.camera;

			const char* projectionTypeStrings[] = { u8"透视投影",u8"正交投影" };
			const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
			if (ImGui::BeginCombo(u8"投影", currentProjectionTypeString))
			{
				for (std::uint32_t i = 0; i < 2; ++i)
				{
					bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
					if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
					{
						currentProjectionTypeString = projectionTypeStrings[i];
						camera.SetProjctionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoGraphicSize = camera.GetOrthoGraphicSize();
				if (ImGui::DragFloat(u8"大小", &orthoGraphicSize))
				{
					camera.SetOrthoGraphicSize(orthoGraphicSize);
				}

				float orthoGraphicNear = camera.GetOrthoGraphicNearClip();
				if (ImGui::DragFloat(u8"近平面", &orthoGraphicNear))
				{
					camera.SetOrthoGraphicNearClip(orthoGraphicNear);
				}

				float orthoGraphicFar = camera.GetOrthoGraphicFarClip();
				if (ImGui::DragFloat(u8"远平面", &orthoGraphicFar))
				{
					camera.SetOrthoGraphicFarClip(orthoGraphicFar);
				}
			}

			if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveFOV = camera.GetPerspectiveFOV();
				if (ImGui::DragFloat(u8"视场角", &perspectiveFOV))
				{
					camera.SetPerspectiveFOV(perspectiveFOV);
				}

				float perspectiveNear = camera.GetPerspectiveNear();
				if (ImGui::DragFloat(u8"近平面", &perspectiveNear))
				{
					camera.SetPerspectiveNear(perspectiveNear);
				}

				float perspectiveFar = camera.GetPerspectiveFar();
				if (ImGui::DragFloat(u8"远平面", &perspectiveFar))
				{
					camera.SetPerspectiveFar(perspectiveFar);
				}
			}
			});

		DrawComponent<MeshComponent>(u8"Mesh", entity, [](MeshComponent& meshComponent) {
			ImGui::ColorEdit4(u8"基础色", glm::value_ptr(meshComponent.color));
			});
	}
}