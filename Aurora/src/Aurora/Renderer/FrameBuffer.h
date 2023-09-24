#pragma once
#include "Aurora/Core/Core.h"
namespace Aurora
{
	enum class FrameBufferTextureFormat
	{
		NONE,
		RGBA8,
		REDINTEGER,
		DEPTH24STENCIL8,
		DEPTH = DEPTH24STENCIL8
	};

	struct FrameBufferTextureProps
	{
		FrameBufferTextureProps() :textureFormat(FrameBufferTextureFormat::NONE)
		{

		}

		FrameBufferTextureProps(FrameBufferTextureFormat textureFormatIn) :textureFormat(textureFormatIn)
		{

		}
		FrameBufferTextureFormat textureFormat;
	};

	struct FrameBufferAttachmentsProps
	{
		FrameBufferAttachmentsProps()
		{

		}
		FrameBufferAttachmentsProps(std::initializer_list<FrameBufferTextureProps> attachmentsIn) :attachments(attachmentsIn)
		{

		}

		std::vector<FrameBufferTextureProps> attachments;
	};

	struct FrameBufferProps
	{
		std::uint32_t width;
		std::uint32_t height;
		std::uint32_t samples;
		FrameBufferAttachmentsProps attachments;

	};
	class AURORA_API FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {};

		virtual void Resize(std::uint32_t width, std::uint32_t height) = 0;
		virtual int  ReadPixel(std::uint32_t colorAttachmentIndex, std::uint32_t x, std::uint32_t y)const=0;
		virtual const FrameBufferProps& GetFrameBufferProps()const = 0;
		virtual std::uint32_t GetColorAttachmentID(std::uint32_t index=0)const = 0;
		virtual void ClearColorAttachment(std::uint32_t colorAttachmentIndex, int value)=0;
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
	public:
		static Ref<FrameBuffer> Create(const FrameBufferProps& props);

	};
}