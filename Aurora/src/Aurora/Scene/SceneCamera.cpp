#include "pch.h"
#include "SceneCamera.h"
#include "gtc/matrix_transform.hpp"

namespace Aurora
{
	SceneCamera::SceneCamera() :m_aspectRatio(1.0f), m_orthoGraphicNear(-1.0f), m_orthoGraphicFar(1.0f), m_orthoGraphicSize(10), m_projectionType(ProjectionType::Orthographic),m_perspectiveNear(0.01f),m_perspectiveFar(1000.0f),m_perspectiveFOV(45.0f)
	{
		ReCalcProjection();
	}

	SceneCamera::~SceneCamera()
	{

	}

	void SceneCamera::SetOrthoGraphic(float size, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::Orthographic;
		m_orthoGraphicSize = size;
		m_orthoGraphicNear = nearClip;
		m_orthoGraphicFar = farClip;
		ReCalcProjection();
	}

	void SceneCamera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_projectionType = ProjectionType::Perspective;
		m_perspectiveFOV = fov;
		m_perspectiveNear = nearClip;
		m_perspectiveFar = farClip;
		ReCalcProjection();
	}

	void SceneCamera::SetViewportSize(std::uint32_t width, std::uint32_t height)
	{
		m_aspectRatio = float(width) / float(height);
		ReCalcProjection();
	}

	void SceneCamera::ReCalcProjection()
	{
		if (m_projectionType == ProjectionType::Orthographic)
		{
			float left = -m_aspectRatio * m_orthoGraphicSize * 0.5f;
			float right = m_aspectRatio * m_orthoGraphicSize * 0.5f;
			float bottom = -m_orthoGraphicSize * 0.5f;
			float top = m_orthoGraphicSize * 0.5;

			m_projection = glm::ortho(left, right, bottom, top, m_orthoGraphicNear, m_orthoGraphicFar);
		}
		else if (m_projectionType == ProjectionType::Perspective)
		{
			m_projection = glm::perspective(m_perspectiveFOV, m_aspectRatio, m_perspectiveNear, m_perspectiveFar);
		}
	}

}