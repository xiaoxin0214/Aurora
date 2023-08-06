#pragma once
#include "Aurora/Core.h"
namespace Aurora
{
	class AURORA_API GraphicsContext
	{
	public:
		virtual ~GraphicsContext() {};
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}