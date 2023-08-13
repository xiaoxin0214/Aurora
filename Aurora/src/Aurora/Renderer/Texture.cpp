#include "pch.h"
#include "Texture.h"
#include "RendererAPI.h"
#include "Platform/OpenGL/OpenGLTexture.h"
namespace Aurora
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		RendererAPI::API currentApi = RendererAPI::GetCurrentAPI();
		if (RendererAPI::API::None==currentApi)
		{
			AURORA_CORE_ASSERT(false, "RendererAPI::None 暂不支持!");
			return NULL;
		}
		else if (RendererAPI::API::OpenGL == currentApi)
		{
			return std::make_shared<OpenGLTexture2D>(path);
		}

		AURORA_CORE_ASSERT(false,"错误的RenderAPI");
		return NULL;
	}
}