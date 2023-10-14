#pragma once
#include "Aurora/Renderer/UniformBuffer.h"

namespace Aurora
{
	class AURORA_API OpenGLUniformBuffer :public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(std::uint32_t size,std::uint32_t binding);
		virtual ~OpenGLUniformBuffer();
	public:
		void SetData(const void* data, std::uint32_t size, std::uint32_t offset = 0)override;

	private:
		uint32_t m_uniformBufferID = 0;
	};
}