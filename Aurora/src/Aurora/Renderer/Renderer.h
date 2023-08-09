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

		static void BeginScene(const OrthographicCamera&camera);
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray,const std::shared_ptr<Shader>&shader);
		static void EndScene();
	private:
		struct SceneData
		{
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* s_pSceneData;
	};
}