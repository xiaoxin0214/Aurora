#pragma once
#include "Aurora/Renderer/VertexBuffer.h"

namespace Aurora
{
	class AURORA_API OpenGLVertexBuffer :public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float*vertices,std::uint32_t size);
		~OpenGLVertexBuffer();
	public:
		void Bind()const override;
		void UnBind()const override;
	private:
		unsigned int m_vertexBufferID;
	};
}