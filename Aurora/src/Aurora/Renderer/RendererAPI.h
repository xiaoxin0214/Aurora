#pragma once
#include "Aurora/Core.h"
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
		virtual void SetClearColor(const glm::vec4&color)=0;
		virtual void Clear()=0;
		virtual void DrawIndexed(const Ref<VertexArray>&vertexArray) = 0;
		static API GetCurrentAPI()
		{
			return s_currentRendererAPI;
		}
	private:
		static API s_currentRendererAPI;
	};
}