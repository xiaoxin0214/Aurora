#pragma once
#include "Aurora/Core/Core.h"
#include "entt.hpp"
#include "Aurora/Core/Timestep.h"
namespace Aurora
{
	class Entity;
	class AURORA_API Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(Timestep ts);
		void OnViewportResize(std::uint32_t width,std::uint32_t height);
		Entity CreateEntity(const std::string&tag);
	private:
		entt::registry m_registry;
		std::uint32_t m_viewportWidth;
		std::uint32_t m_viewportHeight;

		friend class Entity;
	};
}