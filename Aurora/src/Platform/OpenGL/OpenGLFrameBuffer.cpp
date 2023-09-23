#include "pch.h"
#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"
namespace Aurora
{
	static bool IsDepthFormat(FrameBufferTextureFormat format)
	{
		if (FrameBufferTextureFormat::DEPTH == format)
		{
			return true;
		}

		return false;
	}

	static GLenum TextureTarget(bool isMultiSampled)
	{
		return isMultiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
	}

	static void CreateTextures(bool isMultiSampled,std::uint32_t* pOutID,std::uint32_t count)
	{
		glCreateTextures(TextureTarget(isMultiSampled), count, pOutID);
	}

	static void BindTexture(bool isMultiSampled,std::uint32_t id)
	{
		glBindTexture(TextureTarget(isMultiSampled),id);
	}

	static void AttachColorTexture(std::uint32_t id,GLenum format,std::uint32_t samples,std::uint32_t width,std::uint32_t height,int index)
	{
		bool isMultiSampled = samples > 1;
		if (isMultiSampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,samples,format,width,height,GL_FALSE);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, TextureTarget(isMultiSampled), id, 0);
	}

	static void AttachDepthTexture(std::uint32_t id, GLenum format,GLenum attachmentType, std::uint32_t samples, std::uint32_t width, std::uint32_t height)
	{
		bool isMultiSampled = samples > 1;
		if (isMultiSampled)
		{
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
		}
		else
		{
			glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(isMultiSampled), id, 0);
	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferProps& props) :m_props(props),m_frameBufferID(0),m_depthAttachment(0)
	{
		for (auto attachment : props.attachments.attachments)
		{
			if (!IsDepthFormat(attachment.textureFormat))
			{
				m_colorsAttachmentProps.emplace_back(attachment);
			}
			else
			{
				m_depthAttachmentProps = attachment;
			}

		}
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		glDeleteTextures(1, &m_depthAttachment);
		glDeleteFramebuffers(1, &m_frameBufferID);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (!m_colorAttachments.empty())
		{
			glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
			m_colorAttachments.clear();
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

		bool isMultiSampled = m_props.samples > 1;
		if (!m_colorsAttachmentProps.empty())
		{
			m_colorAttachments.resize(m_colorsAttachmentProps.size());
			CreateTextures(isMultiSampled, m_colorAttachments.data(),m_colorsAttachmentProps.size());

			for (std::uint32_t i=0;i< m_colorAttachments.size();++i)
			{
				BindTexture(isMultiSampled, m_colorAttachments[i]);
				switch (m_colorsAttachmentProps[i].textureFormat)
				{
				case FrameBufferTextureFormat::RGBA8:
					{
						AttachColorTexture(m_colorAttachments[i],GL_RGBA,m_props.samples,m_props.width,m_props.height,i);
						break;
					}
				}
			}
		}

		if (m_depthAttachmentProps.textureFormat != FrameBufferTextureFormat::NONE)
		{
			CreateTextures(isMultiSampled,&m_depthAttachment, 1);
			BindTexture(isMultiSampled, m_depthAttachment);
			switch (m_depthAttachmentProps.textureFormat)
			{
			case FrameBufferTextureFormat::DEPTH:
			{
				AttachDepthTexture(m_depthAttachment, GL_DEPTH24_STENCIL8, m_props.samples, m_props.width, m_props.height, 0);
				break;
			}
			}
		}

		if (!m_colorAttachments.empty())
		{
			AURORA_CORE_ASSERT(m_colorAttachments.size()<=4, "colorAttachment个数必须小于等于4!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1 ,GL_COLOR_ATTACHMENT2 ,GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_colorAttachments.size(), buffers);
		}
		else
		{
			glDrawBuffer(GL_NONE);
		}

		AURORA_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "FrameBuffer创建失败!");
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