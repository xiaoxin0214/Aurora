#pragma once
#include "Aurora/Renderer/VertexBuffer.h"

namespace Aurora
{
	class AURORA_API OpenGLVertexBuffer :public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(std::uint32_t size);
		OpenGLVertexBuffer(float*vertices,std::uint32_t size);
		~OpenGLVertexBuffer();
	public:
		void Bind()const override;
		void UnBind()const override;
		void SetData(const void*vertices,std::uint32_t size)override;
	private:
		std::uint32_t m_vertexBufferID;
	};
}