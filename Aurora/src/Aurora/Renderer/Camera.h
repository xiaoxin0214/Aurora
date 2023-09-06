#pragma once
#include "Aurora/Core/Core.h"
#include "glm.hpp"
namespace Aurora
{
	class AURORA_API Camera
	{
	public:
		Camera():m_projection(glm::mat4(1.0f))
		{

		}

		virtual ~Camera()
		{

		}

		Camera(const glm::mat4& projection) :m_projection(projection)
		{

		}

		const glm::mat4& GetProjection()const
		{
			return m_projection;
		}

		glm::mat4& GetProjection()
		{
			return m_projection;
		}
	protected:
		glm::mat4 m_projection;
	};
}