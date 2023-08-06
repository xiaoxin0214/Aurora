#pragma once
#include "Aurora/Core.h"
namespace Aurora
{
	enum class AURORA_API RendererAPI
	{
		None,
		OpenGL
	};
	class AURORA_API Renderer
	{
	public:
		static RendererAPI GetCurrentAPI()
		{
			return s_currentRendererAPI;
		}
	private:
		static RendererAPI s_currentRendererAPI;
	};
}