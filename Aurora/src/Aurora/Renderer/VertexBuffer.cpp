#include "pch.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"
namespace Aurora
{
	VertexBuffer* VertexBuffer::Create(std::uint32_t size)
	{
		RendererAPI::API currentAPI = Renderer::GetCurrentAPI();
		if (RendererAPI::API::None == currentAPI)
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None 暂不支持！");
			return NULL;
		}
		else if (RendererAPI::API::OpenGL == currentAPI)
		{
			return new OpenGLVertexBuffer(size);
		}
		else
		{
			AURORA_CORE_ASSERT(false, "错误的RendererAPI!");
			return NULL;
		}

	}

	VertexBuffer* VertexBuffer::Create(float* vertices, std::uint32_t size)
	{
		RendererAPI::API currentAPI = Renderer::GetCurrentAPI();
		if (RendererAPI::API::None == currentAPI)
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None 暂不支持！");
			return NULL;
		}
		else if (RendererAPI::API::OpenGL == currentAPI)
		{
			return new OpenGLVertexBuffer(vertices, size);
		}
		else
		{
			AURORA_CORE_ASSERT(false, "错误的RendererAPI!");
			return NULL;
		}

	}
}