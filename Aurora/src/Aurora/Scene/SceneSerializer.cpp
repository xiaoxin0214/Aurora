#include "pch.h"
#include "Entity.h"
#include "Components.h"
#include "SceneSerializer.h"
#include "yaml-cpp/yaml.h"

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Aurora
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	static void SerializeEntity(Entity entity, YAML::Emitter& out)
	{
		out << YAML::BeginMap;
		out << YAML::Key << u8"Entity";
		out << YAML::Value << entity.GetID();

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>();
			out << YAML::Key << u8"TagComponent";
			out << YAML::BeginMap;
			out << YAML::Key << u8"Tag";
			out << YAML::Value << tag.tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			out << YAML::Key << u8"TransformComponent";
			out << YAML::BeginMap;
			out << YAML::Key << u8"Position" << YAML::Value << transform.position;
			out << YAML::Key << u8"Rotation" << YAML::Value << transform.rotation;
			out << YAML::Key << u8"Scale" << YAML::Value << transform.scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& camera = entity.GetComponent<CameraComponent>();
			out << YAML::Key << u8"CameraComponent";
			out << YAML::BeginMap;

			out << YAML::Key << u8"Camera";
			out << YAML::BeginMap;
			out << YAML::Key << u8"ProjectionType" << YAML::Value << (int)camera.camera.GetProjectionType();
			out << YAML::Key << u8"PerspectiveFOV" << YAML::Value << camera.camera.GetPerspectiveFOV();
			out << YAML::Key << u8"PerspectiveNear" << YAML::Value << camera.camera.GetPerspectiveNear();
			out << YAML::Key << u8"PerspectiveFar" << YAML::Value << camera.camera.GetPerspectiveFar();
			out << YAML::Key << u8"OrthographicSize" << YAML::Value << camera.camera.GetOrthoGraphicSize();
			out << YAML::Key << u8"OrthographicNear" << YAML::Value << camera.camera.GetOrthoGraphicNearClip();
			out << YAML::Key << u8"OrthographicFar" << YAML::Value << camera.camera.GetOrthoGraphicFarClip();
			out << YAML::EndMap;

			out << YAML::Key << u8"IsMainCamera" << YAML::Value << camera.isMainCamera;
			out << YAML::Key << u8"IsFixedAspectRatio" << YAML::Value << camera.isFixedAspectRatio;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<MeshComponent>())
		{
			auto& meshComponent = entity.GetComponent<MeshComponent>();
			out << YAML::Key << u8"MeshComponent";
			out << YAML::BeginMap;
			out << YAML::Key << u8"Color" << YAML::Value << meshComponent.color;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << u8"Scene";
		out << YAML::Value << u8"Untitled";
		out << YAML::Key << u8"Entities";
		out << YAML::BeginSeq;

		m_scene->m_registry.each([&](auto entityID) {

			Entity entity = { entityID,m_scene.get() };
			if (!entity)
				return;
			SerializeEntity(entity, out);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::string& filePath)
	{
		std::ifstream fin(filePath);
		std::stringstream strStream;
		strStream << fin.rdbuf();

		YAML::Node dataNode = YAML::Load(strStream);
		if (!dataNode[u8"Scene"])
			return false;

		std::string sceneName = dataNode[u8"Scene"].as<std::string>();

		auto& entitiesNode = dataNode[u8"Entities"];
		if (entitiesNode)
		{
			for (auto& entityNode : entitiesNode)
			{
				std::string name;
				std::uint64_t uuid = entityNode[u8"Entity"].as<std::uint64_t>();
				auto& tagNode = entityNode[u8"TagComponent"];
				if (tagNode)
				{
					name = tagNode[u8"Tag"].as<std::string>();
				}

				Entity deserializedEntity = m_scene->CreateEntity(name);

				auto& transformNode = entityNode[u8"TransformComponent"];
				if (transformNode)
				{
					auto& transformComponent = deserializedEntity.GetComponent<TransformComponent>();
					transformComponent.position = transformNode[u8"Position"].as<glm::vec3>();
					transformComponent.rotation = transformNode[u8"Rotation"].as<glm::vec3>();
					transformComponent.scale = transformNode[u8"Scale"].as<glm::vec3>();
				}

				auto& cameraNode = entityNode[u8"CameraComponent"];
				if (cameraNode)
				{
					auto& cameraComponent = deserializedEntity.AddComponent<CameraComponent>();
					auto& cameraPropsNode = cameraNode[u8"Camera"];
					if (cameraPropsNode)
					{
						cameraComponent.camera.SetProjctionType((SceneCamera::ProjectionType)cameraPropsNode[u8"ProjectionType"].as<int>());
						cameraComponent.camera.SetPerspectiveFOV(cameraPropsNode[u8"PerspectiveFOV"].as<float>());
						cameraComponent.camera.SetPerspectiveNear(cameraPropsNode[u8"PerspectiveNear"].as<float>());
						cameraComponent.camera.SetPerspectiveFar(cameraPropsNode[u8"PerspectiveFar"].as<float>());
						cameraComponent.camera.SetOrthoGraphicSize(cameraPropsNode[u8"OrthographicSize"].as<float>());
						cameraComponent.camera.SetOrthoGraphicNearClip(cameraPropsNode[u8"OrthographicNear"].as<float>());
						cameraComponent.camera.SetOrthoGraphicFarClip(cameraPropsNode[u8"OrthographicFar"].as<float>());
					}

					cameraComponent.isMainCamera = cameraNode[u8"IsMainCamera"].as<bool>();
					cameraComponent.isFixedAspectRatio = cameraNode[u8"IsFixedAspectRatio"].as<bool>();
				}

				auto& meshNode = entityNode[u8"MeshComponent"];
				if (meshNode)
				{
					auto& meshComponent = deserializedEntity.AddComponent<MeshComponent>();
					meshComponent.color = meshNode[u8"Color"].as<glm::vec4>();
				}
			}
		}
		return true;
	}
}