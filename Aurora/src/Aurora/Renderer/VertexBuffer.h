#pragma once
#include "Aurora/Core.h"

namespace Aurora
{
	class AURORA_API VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {};
	public:
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
	public:
		static VertexBuffer* Create(float* vertices, std::uint32_t size);
	};
}