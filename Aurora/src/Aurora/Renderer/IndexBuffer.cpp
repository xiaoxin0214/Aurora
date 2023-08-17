#include "pch.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
namespace Aurora
{
	IndexBuffer* IndexBuffer::Create(unsigned int* indices, std::uint32_t count)
	{
		RendererAPI::API currentAPI = Renderer::GetCurrentAPI();
		if (RendererAPI::API::None == currentAPI)
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None 暂不支持！");
			return NULL;
		}
		else if (RendererAPI::API::OpenGL == currentAPI)
		{
			return new OpenGLIndexBuffer(indices, count);
		}
		else
		{
			AURORA_CORE_ASSERT(false, "错误的RendererAPI!");
			return NULL;
		}
	}
}