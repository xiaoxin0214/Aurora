#include "pch.h"
#include "SceneCamera.h"
#include "gtc/matrix_transform.hpp"

namespace Aurora
{
	SceneCamera::SceneCamera() :m_aspectRatio(1.0f), m_orthoGraphicNear(-1.0f), m_orthoGraphicFar(1.0f), m_orthoGraphicSize(10)
	{
		ReCalcProjection();
	}

	SceneCamera::~SceneCamera()
	{

	}

	void SceneCamera::SetOrthoGraphic(float size, float nearClip, float farClip)
	{
		m_orthoGraphicSize = size;
		m_orthoGraphicNear = nearClip;
		m_orthoGraphicFar = farClip;
		ReCalcProjection();
	}
	void SceneCamera::SetViewportSize(std::uint32_t width, std::uint32_t height)
	{
		m_aspectRatio = float(width) / float(height);
		ReCalcProjection();
	}

	void SceneCamera::ReCalcProjection()
	{
		float left = -m_aspectRatio * m_orthoGraphicSize * 0.5f;
		float right = m_aspectRatio * m_orthoGraphicSize * 0.5f;
		float bottom = -m_orthoGraphicSize * 0.5f;
		float top = m_orthoGraphicSize * 0.5;

		m_projection = glm::ortho(left, right, bottom, top, m_orthoGraphicNear, m_orthoGraphicFar);
	}

}