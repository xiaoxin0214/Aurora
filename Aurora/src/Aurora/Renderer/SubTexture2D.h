#pragma once
#include "Aurora/Core/Core.h"
#include "Aurora/Renderer/Texture.h"
#include "glm.hpp"
namespace Aurora
{
	class AURORA_API SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture>&texture,const glm::vec2&min,const glm::vec2&max);

		const Ref<Texture> GetTexture2D()const
		{
			return m_texture;
		}

		const glm::vec2* GetTextureCoords()const
		{
			return m_texCoords;
		}
		static Ref<SubTexture2D> Create(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max)
		{
			return CreateRef<SubTexture2D>(texture,min,max);
		}

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture>& texture, const glm::vec2& coords, const glm::vec2&cellSize,const glm::vec2& spriteSize = glm::vec2(1.0f,1.0f));
	private:
		Ref<Texture> m_texture;
		glm::vec2      m_texCoords[4];
	};
}