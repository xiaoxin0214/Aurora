#include "pch.h"
#include "Entity.h"
namespace Aurora
{
	Entity::Entity() :m_handle(entt::null), m_pScene(NULL)
	{

	}
	Entity::Entity(entt::entity handle, Scene* pScene):m_handle(handle),m_pScene(pScene)
	{

	}

	Entity::~Entity()
	{
		m_handle = entt::null;
		m_pScene = NULL;
	}
}