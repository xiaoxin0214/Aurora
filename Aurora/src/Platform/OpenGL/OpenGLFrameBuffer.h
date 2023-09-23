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

		std::uint32_t GetColorAttachmentID(std::uint32_t index=0)const override
		{
			return m_colorAttachments[index];
		}

	private:
		FrameBufferProps m_props;
		std::uint32_t    m_frameBufferID;
		std::uint32_t    m_depthAttachment;
		std::vector<FrameBufferTextureProps> m_colorsAttachmentProps;
		FrameBufferTextureProps              m_depthAttachmentProps;
		std::vector<std::uint32_t>           m_colorAttachments;
	};
}