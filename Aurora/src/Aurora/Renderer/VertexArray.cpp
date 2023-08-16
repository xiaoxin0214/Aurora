#include "pch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Aurora
{
	Ref<VertexArray> VertexArray::Create()
	{
		if (RendererAPI::API::None == Renderer::GetCurrentAPI())
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None 暂不支持！");
			return NULL;
		}
		else if(RendererAPI::API::OpenGL==Renderer::GetCurrentAPI())
		{
			return  std::make_shared<OpenGLVertexArray>();
		}

		AURORA_CORE_ASSERT(false, "错误的RendererAPI!");
		return NULL;	
	}
}