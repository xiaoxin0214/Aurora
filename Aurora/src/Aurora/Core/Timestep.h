#pragma once
#include "Aurora/Core/Core.h"
namespace Aurora
{
	class AURORA_API Timestep
	{
	public:
		Timestep(float time) :m_time(time)
		{

		}

		operator float()const {
			return m_time;
		}

		float GetSeconds()const
		{
			return m_time;
		}

		float GetMilliSeconds()const
		{
			return m_time * 1000.0f;
		}
	private:
		float m_time;
	};
}