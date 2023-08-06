#include "pch.h"
#include "OpenGLIndexBuffer.h"
#include "glad/glad.h"
namespace Aurora
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(unsigned int* indices, std::uint32_t count)
	{
		m_count = count;
		glCreateBuffers(1, &m_indexBufferID);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_indexBufferID);
	}

	void OpenGLIndexBuffer::Bind()const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_indexBufferID);
	}

	void OpenGLIndexBuffer::UnBind()const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
	}

	std::uint32_t OpenGLIndexBuffer::GetCount()const
	{
		return m_count;
	}
}