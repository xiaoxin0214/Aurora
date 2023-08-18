#pragma once
#include "Aurora/Core/Core.h"
#include "Aurora/Renderer/OrthographicCamera.h"
#include "Texture.h"

namespace Aurora
{
	class AURORA_API Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size,float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, float rotation,const Ref<Texture>&texture, const glm::vec4& tintColor);
		static void EndScene();
	};
}