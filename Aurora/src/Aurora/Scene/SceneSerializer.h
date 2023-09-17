#pragma once
#include "Aurora/Core/Core.h"
#include "Scene.h"

namespace Aurora
{
	class AURORA_API SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene) :m_scene(scene)
		{

		}

		~SceneSerializer()
		{

		}

	public:
		void Serialize(const std::string&filePath);
		bool Deserialize(const std::string&filePath);

	private:
		Ref<Scene>   m_scene;
	};
}