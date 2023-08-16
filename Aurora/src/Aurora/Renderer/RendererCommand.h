#pragma once
#include "Aurora/Core/Core.h"
#include "RendererAPI.h"
namespace Aurora
{
	class AURORA_API RendererCommand
	{
	public:
		static void Init()
		{
			s_pRendererAPI->Init();
		}
		static void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)
		{
			s_pRendererAPI->SetViewport(x,y,width,height);
		}
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