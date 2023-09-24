#pragma once
#include "Aurora/Renderer/IndexBuffer.h"

namespace Aurora
{
	class AURORA_API OpenGLIndexBuffer :public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* indices, std::uint32_t count);
		~OpenGLIndexBuffer();
	public:
		void Bind()const override;
		void UnBind()const override;
		std::uint32_t GetCount()const override;
	private:
		std::uint32_t m_indexBufferID;
		std::uint32_t m_count;
	};
}