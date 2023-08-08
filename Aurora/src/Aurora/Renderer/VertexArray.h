#pragma once
#include "Aurora/Core.h"
#include "Aurora/Renderer/VertexBuffer.h"
#include "Aurora/Renderer/IndexBuffer.h"
#include <memory.h>
namespace Aurora
{
	class AURORA_API VertexArray
	{
	public:
		virtual ~VertexArray() {};
	public:
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& vb) = 0;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& ib) = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer()const = 0;
	public:
		static VertexArray* Create();
	};
}