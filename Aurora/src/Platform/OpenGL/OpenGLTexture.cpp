#include "pch.h"
#include "Aurora/Log.h"
#include "OpenGLTexture.h"
#include "stb_image.h"
#include "glad/glad.h"
namespace Aurora
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
	{
		int width = 0;
		int height = 0;
		int channels = 0;
		stbi_set_flip_vertically_on_load(true);
		stbi_uc* pData = stbi_load(path.c_str(), &width, &height, &channels, 0);
		AURORA_CORE_ASSERT(pData, "º”‘ÿÕº∆¨({0}) ß∞‹!", path.c_str());

		m_width = width;
		m_height = height;
		m_texturePath = path;
		glCreateTextures(GL_TEXTURE_2D, 1, &m_textureID);
		glTextureStorage2D(m_textureID, 1, GL_RGB8, m_width, m_height);
		glTextureParameteri(m_textureID,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTextureParameteri(m_textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureSubImage2D(m_textureID, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, pData);

		stbi_image_free(pData);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_textureID);
	}

	void OpenGLTexture2D::Bind(std::uint32_t slot)const
	{
		glBindTextureUnit(slot, m_textureID);
	}
}