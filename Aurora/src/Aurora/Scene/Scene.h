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
		Entity CreateEntity(const std::string&tag);
	private:
		entt::registry m_registry;

		friend class Entity;
	};
}