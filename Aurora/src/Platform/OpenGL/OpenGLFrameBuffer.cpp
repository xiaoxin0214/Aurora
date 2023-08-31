#include "pch.h"
#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"
namespace Aurora
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProps& props) :m_props(props),m_frameBufferID(0),m_colorAttachment(0),m_depthAttachment(0)
	{
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteTextures(1, &m_colorAttachment);
		glDeleteTextures(1, &m_depthAttachment);
		glDeleteFramebuffers(1, &m_frameBufferID);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_colorAttachment != 0)
		{
			glDeleteTextures(1, &m_colorAttachment);
		}
		if (m_depthAttachment != 0)
		{
			glDeleteTextures(1, &m_depthAttachment);
		}

		if (m_frameBufferID!=0)
		{
			glDeleteFramebuffers(1, &m_frameBufferID);
		}

		glCreateFramebuffers(1, &m_frameBufferID);
		Bind();

		glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
		glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_props.width, m_props.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_props.width, m_props.height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

		AURORA_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer´´½¨Ê§°Ü!");
		UnBind();
	}

	void OpenGLFrameBuffer::Resize(std::uint32_t width,std::uint32_t height)
	{
		m_props.width = width;
		m_props.height = height;
		Invalidate();

	}
	void OpenGLFrameBuffer::Bind()const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
		glViewport(0,0,m_props.width,m_props.height);
	}

	void OpenGLFrameBuffer::UnBind()const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}