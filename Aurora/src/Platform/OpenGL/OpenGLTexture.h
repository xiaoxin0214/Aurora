#pragma once
#include "Aurora/Renderer/Texture.h"
namespace Aurora
{
	class AURORA_API OpenGLTexture2D :public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string&path);
		~OpenGLTexture2D();
	public:
		std::uint32_t GetWidth()const override
		{
			return m_width;
		}
		std::uint32_t GetHeight()const override
		{
			return m_height;
		}
		void Bind(std::uint32_t slot)const override;
		bool operator ==(const Texture& rhs)const override
		{
			return m_textureID == ((OpenGLTexture2D&)rhs).m_textureID;
		}
	private:
		std::uint32_t m_width;
		std::uint32_t m_height;
		std::uint32_t m_textureID;
		std::string   m_texturePath;
	};
}