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
		bool opened = ImGui::TreeNodeEx((void*)(entity.GetID()), flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_selectedEntity = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
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
			if (ImGui::InputText(u8"标签", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, u8"变换"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().transform;
				ImGui::DragFloat3(u8"位置", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}


		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, u8"相机"))
			{
				auto& camera = entity.GetComponent<CameraComponent>().camera;

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
					if(ImGui::DragFloat(u8"大小", &orthoGraphicSize))
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
					float perspectiveFOV = glm::degrees(camera.GetPerspectiveFOV());
					if (ImGui::DragFloat(u8"视场角", &perspectiveFOV))
					{
						camera.SetPerspectiveFOV(glm::radians(perspectiveFOV));
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

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<MeshComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(MeshComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, u8"Mesh"))
			{
				auto& color= entity.GetComponent<MeshComponent>().color;
				ImGui::ColorEdit4(u8"基础色",glm::value_ptr(color));
				ImGui::TreePop();
			}
		}
	}
}