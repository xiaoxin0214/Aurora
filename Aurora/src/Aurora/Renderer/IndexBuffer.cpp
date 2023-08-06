#include "pch.h"
#include "Renderer.h"
#include "Aurora/Log.h"
#include "IndexBuffer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"
namespace Aurora
{
	IndexBuffer* IndexBuffer::Create(unsigned int* indices, std::uint32_t count)
	{
		RendererAPI currentAPI = Renderer::GetCurrentAPI();
		if (RendererAPI::None == currentAPI)
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None �ݲ�֧�֣�");
			return NULL;
		}
		else if (RendererAPI::OpenGL == currentAPI)
		{
			return new OpenGLIndexBuffer(indices, count);
		}
		else
		{
			AURORA_CORE_ASSERT(false, "�����RendererAPI!");
			return NULL;
		}
	}
}