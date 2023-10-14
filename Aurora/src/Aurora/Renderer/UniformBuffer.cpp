#include "pch.h"
#include "UniformBuffer.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"
#include "Aurora/Core/Log.h"

namespace Aurora
{
	Ref<UniformBuffer> UniformBuffer::Create(std::uint32_t size, std::uint32_t binding)
	{
		auto currentApi=RendererAPI::GetCurrentAPI();
		if (RendererAPI::API::None == currentApi)
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None 暂不支持！");
			return NULL;
		}
		else if (RendererAPI::API::OpenGL == currentApi)
		{
			return CreateRef<OpenGLUniformBuffer>(size,binding);
		}
		else
		{
			AURORA_CORE_ASSERT(false, "错误的RendererAPI！");
			return NULL;
		}
	}
}