#pragma once
#include "Aurora/Renderer/RendererAPI.h"
namespace Aurora
{
	class AURORA_API OpenGLRendererAPI :public RendererAPI
	{
	public:
		void Init()override;
		void SetClearColor(const glm::vec4& color)override;
		void Clear()override;
		void DrawIndexed(const Ref<VertexArray>&vertexArray)override;
	};
}