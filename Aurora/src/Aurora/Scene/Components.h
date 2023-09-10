#pragma once
#include "glm.hpp"
#include "Aurora/Core/Timestep.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
namespace Aurora
{
	struct TagComponent
	{
		std::string tag;
		TagComponent(const std::string& tagIn) :tag(tagIn)
		{

		}
	};

	struct TransformComponent
	{
		glm::mat4 transform;
		TransformComponent(const glm::mat4& transformIn) :transform(transformIn)
		{

		}

		TransformComponent()
		{

		}

		operator const glm::mat4& () const
		{
			return transform;
		}

		operator glm::mat4& ()
		{
			return transform;
		}

	};

	struct MeshComponent {
		glm::vec4 color;
		MeshComponent(const glm::vec4& colorIn):color(colorIn)
		{

		}
	};

	struct CameraComponent
	{
		SceneCamera camera;
		bool   isFixedAspectRatio;
		bool   isMainCamera;

		CameraComponent() :isFixedAspectRatio(false), isMainCamera(true)
		{

		}
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* pEntity;

		NativeScriptComponent() :pEntity(NULL)
		{

		}

		std::function<void()>   InstantiateScript;
		std::function<void()>   DestroyScript;

		template<typename T>
		void Bind()
		{
			InstantiateScript = [&]() {
				pEntity = new T();
			};

			DestroyScript = [&]() {
				delete pEntity;
				pEntity = NULL;
			};
		}
	};
}