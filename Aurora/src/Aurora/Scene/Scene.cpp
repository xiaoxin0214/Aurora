#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Aurora/Renderer/Renderer2D.h"
namespace Aurora
{
	Scene::Scene() :m_viewportWidth(0), m_viewportHeight(0)
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& tag)
	{
		Entity entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}

	Entity Scene::GetMainCameraEntity()
	{
		auto& group = m_registry.view<CameraComponent>();
		for (auto& entity : group)
		{
			auto& camera = group.get<CameraComponent>(entity);
			if (camera.isMainCamera)
			{
				return { entity,this };
			}
		}

		return {};
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_registry.destroy(entity);
	}

	void Scene::OnUpdate(Timestep ts)
	{

		{
			auto& scriptGroup = m_registry.view<NativeScriptComponent>();
			for (auto& entity : scriptGroup)
			{
				auto& scriptComponent = scriptGroup.get<NativeScriptComponent>(entity);
				if (!scriptComponent.pEntity)
				{
					scriptComponent.InstantiateScript();
					scriptComponent.pEntity->m_entity = Entity(entity, this);
					scriptComponent.pEntity->OnCreate();
				}

				scriptComponent.pEntity->OnUpdate(ts);
			}
		}

		Camera* pCamera = NULL;
		glm::mat4 cameraTransform;
		{
			auto& group = m_registry.view<TransformComponent, CameraComponent>();
			for (auto& entity : group)
			{
				auto& camera = group.get<CameraComponent>(entity);
				if (camera.isMainCamera)
				{
					pCamera = &camera.camera;
					cameraTransform = group.get<TransformComponent>(entity);
					break;
				}
			}
		}

		if (pCamera != NULL)
		{
			Renderer2D::BeginScene(*pCamera, cameraTransform);
			auto& group = m_registry.group<TransformComponent, MeshComponent>();
			for (auto& entity : group)
			{
				auto& transform = group.get<TransformComponent>(entity);
				auto& mesh = group.get<MeshComponent>(entity);
				Renderer2D::DrawQuad(transform, mesh.color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnUpdateEditor(Timestep ts, EditorCamera& editorCamera)
	{
		Renderer2D::BeginScene(editorCamera);
		auto& group = m_registry.group<TransformComponent, MeshComponent>();
		for (auto& entity : group)
		{
			auto& transform = group.get<TransformComponent>(entity);
			auto& mesh = group.get<MeshComponent>(entity);
			Renderer2D::DrawQuad(transform, mesh.color);
		}
		Renderer2D::EndScene();
	}

	void Scene::OnViewportResize(std::uint32_t width, std::uint32_t height)
	{
		m_viewportWidth = width;
		m_viewportHeight = height;
		auto& group = m_registry.view<CameraComponent>();
		for (auto& entity : group)
		{
			auto& camera = group.get<CameraComponent>(entity);
			if (!camera.isFixedAspectRatio)
			{
				camera.camera.SetViewportSize(width, height);
			}
		}
	}
	template<typename T>
	void Scene::OnComponentAdded(T& component, Entity entity)
	{

	}

	template<>
	void Scene::OnComponentAdded(TagComponent& component, Entity entity)
	{
	}

	template<>
	void Scene::OnComponentAdded(MeshComponent& component, Entity entity)
	{
	}

	template<>
	void Scene::OnComponentAdded(TransformComponent& component, Entity entity)
	{
	}

	template<>
	void Scene::OnComponentAdded(NativeScriptComponent& component, Entity entity)
	{
	}

	template<>
	void Scene::OnComponentAdded(CameraComponent& component, Entity entity)
	{
		component.camera.SetViewportSize(m_viewportWidth, m_viewportHeight);
	}
}