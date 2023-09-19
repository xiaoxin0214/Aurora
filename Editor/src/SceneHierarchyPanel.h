#pragma once
#include "Aurora.h"
namespace Aurora
{
	class AURORA_API SceneHierarchyPanel
	{
	public:

		SceneHierarchyPanel()
		{

		}
		SceneHierarchyPanel(Ref<Scene>& context);

		void SetContext(Ref<Scene>& context)
		{
			m_selectedEntity = {};
			m_context = context;
		}

		Entity GetSelectedEntity() const
		{
			return m_selectedEntity;
		}

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		Ref<Scene> m_context;
		Entity     m_selectedEntity;
	};
}