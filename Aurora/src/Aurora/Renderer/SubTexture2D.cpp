#include "pch.h"
#include "Aurora/Renderer/SubTexture2D.h"

namespace Aurora
{
	SubTexture2D::SubTexture2D(const Ref<Texture>& texture, const glm::vec2& min, const glm::vec2& max)
	{
		m_texture = texture;
		m_texCoords[0] = glm::vec2(min.x, min.y);
		m_texCoords[1] = glm::vec2(max.x, min.y);
		m_texCoords[2] = glm::vec2(max.x, max.y);
		m_texCoords[3] = glm::vec2(min.x, max.y);
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture>& texture, const glm::vec2& coords,const glm::vec2&cellSize, const glm::vec2& spriteSize)
	{
		glm::vec2 min{(coords.x* cellSize.x) / texture->GetWidth(), (coords.y* cellSize.y) / texture->GetHeight()};
		glm::vec2 max{((coords.x + spriteSize.x)* cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y)* cellSize.y) / texture->GetHeight()};
		return Create(texture, min, max);
	}
}