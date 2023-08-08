#pragma once
#include "Aurora/Core.h"
#include "RendererAPI.h"
namespace Aurora
{

	class AURORA_API Renderer
	{
	public:
		static RendererAPI::API GetCurrentAPI()
		{
			return RendererAPI::GetCurrentAPI();
		}

		static void BeginScene();
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
		static void EndScene();
	private:

	};
}