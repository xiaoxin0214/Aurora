#include "pch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"
namespace Aurora
{
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferProps& props)
	{
		auto currentApi = Renderer::GetCurrentAPI();
		if (RendererAPI::API::None == currentApi)
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None �ݲ�֧�֣�");
			return NULL;
		}
		else if (RendererAPI::API::OpenGL == currentApi)
		{
			return CreateRef<OpenGLFrameBuffer>(props);
		}

		AURORA_CORE_ASSERT(false,"�����RendererAPI��");
		return NULL;
	}
}