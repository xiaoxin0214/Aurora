#pragma once
#include "Aurora/Core/Core.h"
namespace Aurora
{
	struct FrameBufferProps
	{
		std::uint32_t width;
		std::uint32_t height;
		std::uint32_t samples;
	};
	class AURORA_API FrameBuffer
	{
	public:
		virtual ~FrameBuffer() {};

		virtual void Resize(std::uint32_t width,std::uint32_t height) = 0;
		virtual const FrameBufferProps& GetFrameBufferProps()const = 0;
		virtual std::uint32_t GetColorAttachmentID()const = 0;
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
	public:
		static Ref<FrameBuffer> Create(const FrameBufferProps&props);

	};
}