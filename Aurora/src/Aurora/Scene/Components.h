#pragma once
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "Aurora/Core/Timestep.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>
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
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		TransformComponent(const glm::vec3& translation) :position(translation), rotation(glm::vec3(0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f, 1.0f, 1.0f))
		{

		}

		TransformComponent() :position(glm::vec3(0.0f, 0.0f, 0.0f)), rotation(glm::vec3(0.0f, 0.0f, 0.0f)), scale(glm::vec3(1.0f, 1.0f, 1.0f))
		{

		}

		operator glm::mat4() const
		{
			glm::mat4 rotationMatrix = glm::toMat4(glm::quat(glm::radians(rotation)));
			return glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct MeshComponent {
		glm::vec4 color;
		MeshComponent() :color(glm::vec4(1.0f,0.0f,0.0f,1.0f))
		{

		}
		MeshComponent(const glm::vec4& colorIn) :color(colorIn)
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