#pragma once
#include "Aurora/Core/Core.h"
namespace Aurora
{
	class AURORA_API UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}
		virtual void SetData(const void* data, std::uint32_t size, std::uint32_t offset = 0) = 0;

		static Ref<UniformBuffer> Create(std::uint32_t size, std::uint32_t binding);
	};
}