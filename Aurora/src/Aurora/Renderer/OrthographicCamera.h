#pragma once
#include "Aurora/Core.h"
#include "glm.hpp"
namespace Aurora
{
	class AURORA_API OrthographicCamera
	{
	public:
		OrthographicCamera(float left,float right,float bottom,float top);
		~OrthographicCamera();

		float GetRotation()const
		{
			return m_rotation;
		}

		const glm::vec3 GetPosition()const
		{
			return m_position;
		}

		const glm::mat4& GetViewProjectionMatrix()const
		{
			return m_vpMatrix;
		}

		const glm::mat4& GetViewMatrix()const
		{
			return m_viewMatrix;
		}

		const glm::mat4& GetProjectionMatrix()const
		{
			return m_projMatrix;
		}

		void SetPosition(const glm::vec3&position)
		{
			m_position = position;
			ReCalcViewMatrix();
		}

		void SetRotation(float rotation)
		{
			m_rotation = rotation;
			ReCalcViewMatrix();
		}

	private:
		void ReCalcViewMatrix();
	private:
		glm::mat4 m_vpMatrix;
		glm::mat4 m_viewMatrix;
		glm::mat4 m_projMatrix;
		glm::vec3 m_position;
		float     m_rotation;
	};
}