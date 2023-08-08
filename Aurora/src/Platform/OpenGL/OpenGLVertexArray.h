#pragma once
#include "Aurora/Renderer/VertexArray.h"

namespace Aurora
{
	class AURORA_API OpenGLVertexArray :public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();
	public:
		void Bind()const override;
		void UnBind()const override;
		void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vb)override;
		void SetIndexBuffer(std::shared_ptr<IndexBuffer>& ib)override;
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer()const
		{
			return m_indexBuffer;
		}
	private:
		std::uint32_t m_vertexArrayID;
		std::vector<std::shared_ptr<VertexBuffer>>  m_vertexBufferLst;
		std::shared_ptr<IndexBuffer>                m_indexBuffer;
	};
}