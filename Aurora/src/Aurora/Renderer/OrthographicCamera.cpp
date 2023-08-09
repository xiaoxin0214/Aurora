#include "pch.h"
#include "OrthographicCamera.h"
#include "gtc/matrix_transform.hpp"

namespace Aurora
{
	OrthographicCamera::OrthographicCamera(float left,float right,float bottom,float top)
	{
		m_projMatrix = glm::ortho(left,right,bottom,top,-1.0f,1.0f);
		m_viewMatrix = glm::mat4(1.0f);
		m_position = glm::vec3(0.0);
		m_rotation = 0.0f; 
		m_vpMatrix = m_projMatrix * m_viewMatrix;
	}

	OrthographicCamera::~OrthographicCamera()
	{

	}

	void OrthographicCamera::ReCalcViewMatrix()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0), m_position) * glm::rotate(glm::mat4(1.0), m_rotation, glm::vec3(0.0,0.0,1.0));
		m_viewMatrix = glm::inverse(transform);

		m_vpMatrix = m_projMatrix* m_viewMatrix;
	}
}