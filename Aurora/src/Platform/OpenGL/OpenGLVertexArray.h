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
		void AddVertexBuffer(Ref<VertexBuffer>& vb)override;
		void SetIndexBuffer(Ref<IndexBuffer>& ib)override;
		const Ref<IndexBuffer>& GetIndexBuffer()const
		{
			return m_indexBuffer;
		}
	private:
		std::uint32_t m_vertexArrayID;
		std::vector<Ref<VertexBuffer>>  m_vertexBufferLst;
		Ref<IndexBuffer>                m_indexBuffer;
	};
}