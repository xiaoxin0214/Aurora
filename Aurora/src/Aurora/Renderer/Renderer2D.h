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
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& pos, const glm::vec2& size, const Ref<Texture>&texture);
		static void EndScene();
	};
}