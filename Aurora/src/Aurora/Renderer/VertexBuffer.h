#pragma once
#include "Aurora/Core.h"
#include "BufferLayout.h"
namespace Aurora
{
	class AURORA_API VertexBuffer
	{
	public:
		VertexBuffer() {};
		virtual ~VertexBuffer() {};
	public:
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual void SetLayout(const BufferLayout& layout)
		{
			m_layout = layout;
		}
		virtual const BufferLayout& GetLayout()const
		{
			return m_layout;
		}
	public:
		static VertexBuffer* Create(float* vertices, std::uint32_t size);
	private:
		BufferLayout           m_layout;
	};
}