#pragma once

#include "Aurora/Core/Core.h"
namespace Aurora
{
	class AURORA_API FileDialog
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}