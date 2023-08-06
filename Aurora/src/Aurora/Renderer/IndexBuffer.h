#pragma once
#include "Aurora/Core.h"
namespace Aurora
{
	class AURORA_API IndexBuffer
	{
	public:
		IndexBuffer() {

		}

		virtual ~IndexBuffer() {

		}

	public:
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
		virtual std::uint32_t GetCount()const = 0;
	public:
		static IndexBuffer* Create(unsigned int* indices, std::uint32_t count);
	};
}