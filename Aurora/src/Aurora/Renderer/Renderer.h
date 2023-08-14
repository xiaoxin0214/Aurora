#pragma once
#include "Aurora/Core.h"
#include "RendererAPI.h"
#include "Shader.h"
#include "OrthographicCamera.h"
namespace Aurora
{

	class AURORA_API Renderer
	{
	public:
		static RendererAPI::API GetCurrentAPI()
		{
			return RendererAPI::GetCurrentAPI();
		}

		static void Init();
		static void OnWindowResize(std::uint32_t width,std::uint32_t height);
		static void BeginScene(const OrthographicCamera& camera);
		static void Submit(const Ref<VertexArray>& vertexArray, const Ref<Shader>& shader, const glm::mat4& modelMatrix);
		static void EndScene();
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* s_pSceneData;
	};
}