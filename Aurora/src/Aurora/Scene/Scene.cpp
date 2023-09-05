#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Aurora/Renderer/Renderer2D.h"
namespace Aurora
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string&tag)
	{
		Entity entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto& group = m_registry.group<TransformComponent>();
		for (auto& entity : group)
		{
			auto& transform = group.get<TransformComponent>(entity);
			Renderer2D::DrawQuad(transform,glm::vec4(0.0f,1.0f,1.0f,1.0f));
		}
	}
}