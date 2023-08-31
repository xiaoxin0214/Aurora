#pragma once
#include "Aurora/Renderer/FrameBuffer.h"
namespace Aurora
{
	class AURORA_API OpenGLFrameBuffer :public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferProps& props);
		~OpenGLFrameBuffer();
		void Invalidate();

		void Resize(std::uint32_t width,std::uint32_t height)override;
		void Bind()const override;
		void UnBind()const override;

		const FrameBufferProps& GetFrameBufferProps()const override
		{
			return m_props;
		}

		std::uint32_t GetColorAttachmentID()const override
		{
			return m_colorAttachment;
		}

	private:
		FrameBufferProps m_props;
		std::uint32_t    m_frameBufferID;
		std::uint32_t    m_colorAttachment;
		std::uint32_t    m_depthAttachment;
	};
}