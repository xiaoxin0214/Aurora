#include "pch.h"
#include "OpenGLVertexBuffer.h"
#include "glad/glad.h"
namespace Aurora
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, std::uint32_t size)
	{
		glCreateBuffers(1, &m_vertexBufferID);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1,&m_vertexBufferID);
	}

	void OpenGLVertexBuffer::Bind()const
	{
		glBindBuffer(GL_ARRAY_BUFFER,m_vertexBufferID);
	}

	void OpenGLVertexBuffer::UnBind()const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}