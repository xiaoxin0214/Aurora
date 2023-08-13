#pragma once
#include "Aurora/Core.h"
namespace Aurora
{
	class AURORA_API Texture
	{
	public:
		virtual ~Texture()
		{

		};
	public:
		virtual std::uint32_t GetWidth()const = 0;
		virtual std::uint32_t GetHeight()const = 0;
		virtual void Bind(std::uint32_t slot)const =0;

	};
	
	class AURORA_API Texture2D :public Texture
	{
	public:
		virtual ~Texture2D()
		{

		}

	public:
		static Ref<Texture2D> Create(const std::string& path);
	};
}