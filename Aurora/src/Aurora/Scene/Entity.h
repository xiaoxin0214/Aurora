#pragma once
#include "Aurora/Core/Core.h"
#include "entt.hpp"
#include "Scene.h"
namespace Aurora
{
	class AURORA_API Entity
	{
	public:
		Entity();
		Entity(entt::entity handle, Scene* pScene);
		~Entity();
		template<typename T>
		void HasComponent()
		{
			return m_pScene->m_registry.has<T>(m_handle);
		}

		template<typename T, typename ...Args>
		T& AddComponent(Args&&...args)
		{
			return m_pScene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			return m_pScene->m_registry.get<T>(m_handle);
		}

		template<typename T>
		void RemoveComponent()
		{
			m_pScene->m_registry.remove<T>(m_handle);
		}
	private:
		entt::entity m_handle;
		Scene* m_pScene;
	};
}