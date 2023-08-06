#pragma once

#include "Aurora/Core.h"
#include <string>
namespace Aurora
{
	class AURORA_API Shader
	{
	public:
		Shader() {};
		virtual ~Shader() {};
	public:
		virtual void Bind()const = 0;
		virtual void UnBind()const = 0;
	private:

	};
}