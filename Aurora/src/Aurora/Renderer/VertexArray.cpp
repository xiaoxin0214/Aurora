#include "pch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Aurora
{
	VertexArray* VertexArray::Create()
	{
		if (RendererAPI::API::None == Renderer::GetCurrentAPI())
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None 暂不支持！");
			return NULL;
		}
		else if(RendererAPI::API::OpenGL==Renderer::GetCurrentAPI())
		{
			return new OpenGLVertexArray();
		}

		AURORA_CORE_ASSERT(false, "错误的RendererAPI!");
		return NULL;	
	}
}