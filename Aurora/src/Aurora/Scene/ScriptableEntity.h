#pragma once
#include "Aurora/Core/Core.h"
#include "Entity.h"
namespace Aurora
{
	class AURORA_API ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}
	public:
		virtual ~ScriptableEntity()
		{

		}

		virtual void OnCreate() {}

		virtual void OnUpdate(Timestep ts) {}

		virtual void OnDestroy() {}
	private:
		Entity m_entity;
		friend class Scene;

	};
}