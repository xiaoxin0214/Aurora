#pragma once
#include "Aurora/Core/Core.h"
#include "glm.hpp"
#include <memory>
#include "VertexArray.h"
namespace Aurora {
	class AURORA_API RendererAPI
	{
	public:
		enum class AURORA_API API
		{
			None,
			OpenGL
		};
	public:
		virtual void Init()=0;
		virtual void SetViewport(std::uint32_t x, std::uint32_t y, std::uint32_t width, std::uint32_t height)=0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray,std::uint32_t count = 0) = 0;
		static API GetCurrentAPI()
		{
			return s_currentRendererAPI;
		}
	private:
		static API s_currentRendererAPI;
	};
}