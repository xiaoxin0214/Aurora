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
		bool HasComponent()
		{
			return m_pScene->m_registry.try_get<T>(m_handle)!=nullptr;
		}

		template<typename T, typename ...Args>
		T& AddComponent(Args&&...args)
		{
			T&component= m_pScene->m_registry.emplace<T>(m_handle, std::forward<Args>(args)...);
			m_pScene->OnComponentAdded<T>(component,*this);
			return component;
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

		std::uint32_t GetID()const
		{
			return (std::uint32_t)m_handle;
		}

		bool operator==(const Entity& rhs)const
		{
			return m_handle == rhs.m_handle && m_pScene == rhs.m_pScene;;
		}

		operator bool()const
		{
			return !(m_handle == entt::null);
		}

		operator entt::entity()const
		{
			return m_handle;
		}
	private:
		entt::entity m_handle;
		Scene* m_pScene;
	};
}