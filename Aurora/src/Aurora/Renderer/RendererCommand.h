#pragma once
#include "Aurora/Core.h"
#include "RendererAPI.h"
namespace Aurora
{
	class AURORA_API RendererCommand
	{
	public:
		static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_pRendererAPI->DrawIndexed(vertexArray);
		}

		static void SetClearColor(const glm::vec4& color) 
		{
			s_pRendererAPI->SetClearColor(color);
		}
		static void Clear() 
		{
			s_pRendererAPI->Clear();
		}

	private:
		static RendererAPI* s_pRendererAPI;
	};
}