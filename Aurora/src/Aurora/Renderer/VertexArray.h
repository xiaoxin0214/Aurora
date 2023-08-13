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
		virtual void AddVertexBuffer(Ref<VertexBuffer>& vb) = 0;
		virtual void SetIndexBuffer(Ref<IndexBuffer>& ib) = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer()const = 0;
	public:
		static VertexArray* Create();
	};
}