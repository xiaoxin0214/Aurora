#pragma once
#include "Aurora/Core/Core.h"
#include "Aurora/Renderer/Camera.h"
#include "Aurora/Renderer/OrthographicCamera.h"
#include "Aurora/Renderer/EditorCamera.h"
#include "Texture.h"
#include "Aurora/Renderer/SubTexture2D.h"

namespace Aurora
{
	class AURORA_API Renderer2D
	{
	public:
		struct Statistics
		{
			std::uint32_t drawCalls;
			std::uint32_t quadCount;
			Statistics() :drawCalls(0), quadCount(0)
			{

			}

			std::uint32_t GetVertexCount()
			{
				return quadCount * 4;
			}

			std::uint32_t GetIndexCount()
			{
				return quadCount * 6;
			}
		};
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera&editorCamera);
		static void BeginScene(const OrthographicCamera& camera);
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color,int entityID=-1);
		static void DrawQuad(const glm::mat4& transform, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture>& texture, const glm::vec4& tintColor,int entityID=-1);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size,float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation,const Ref<Texture>&texture, const glm::vec4& tintColor);
		static void DrawQuad(const glm::vec3& pos, const glm::vec2& size, float rotation, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor);
		static void EndScene();
		static void Flush();
		static void ResetStatistics();
		static Renderer2D::Statistics GetStatistics();
	private:
		static void FlushAndReset();
	};
}