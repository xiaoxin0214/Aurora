#include "pch.h"
#include "OpenGLUniformBuffer.h"
#include "glad/glad.h"
namespace Aurora
{
	OpenGLUniformBuffer::OpenGLUniformBuffer(std::uint32_t size, std::uint32_t binding)
	{
		glCreateBuffers(1, &m_uniformBufferID);
		glNamedBufferData(m_uniformBufferID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_uniformBufferID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &m_uniformBufferID);
	}

	void OpenGLUniformBuffer::SetData(const void* data, std::uint32_t size, std::uint32_t offset)
	{
		glNamedBufferSubData(m_uniformBufferID, offset, size, data);
	}
}