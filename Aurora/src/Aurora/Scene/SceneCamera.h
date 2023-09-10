#pragma once
#include "Aurora/Core/Core.h"
#include "Aurora/Renderer/Camera.h"
namespace Aurora
{
	class AURORA_API SceneCamera :public Camera
	{
	public:
		enum class ProjectionType
		{
			Perspective = 0,
			Orthographic = 1
		};
	public:
		SceneCamera();
		virtual ~SceneCamera();
		void SetViewportSize(std::uint32_t width, std::uint32_t height);
		void SetOrthoGraphic(float size, float nearClip, float farClip);
		void SetPerspective(float fov, float nearClip, float farClip);
		void SetOrthoGraphicSize(float size)
		{
			m_orthoGraphicSize = size;
			ReCalcProjection();
		}
		float GetOrthoGraphicSize()const
		{
			return m_orthoGraphicSize;
		}

		float GetOrthoGraphicNearClip()const
		{
			return m_orthoGraphicNear;
		}

		void SetOrthoGraphicNearClip(float nearClip)
		{
			m_orthoGraphicNear = nearClip;
			ReCalcProjection();
		}

		float GetOrthoGraphicFarClip()const
		{
			return m_orthoGraphicFar;
		}

		void SetOrthoGraphicFarClip(float farClip)
		{
			m_orthoGraphicFar = farClip;
			ReCalcProjection();
		}

		ProjectionType GetProjectionType()const
		{
			return m_projectionType;
		}

		void SetProjctionType(ProjectionType type)
		{
			m_projectionType = type;
			ReCalcProjection();
		}

		float GetPerspectiveFOV()const
		{
			return m_perspectiveFOV;
		}

		void SetPerspectiveFOV(float fov)
		{
			m_perspectiveFOV = fov;
			ReCalcProjection();
		}

		float GetPerspectiveNear()const
		{
			return m_perspectiveNear;
		}

		void SetPerspectiveNear(float nearClip)
		{
			m_perspectiveNear = nearClip;
			ReCalcProjection();
		}

		float GetPerspectiveFar()const
		{
			return m_perspectiveFar;
		}

		void SetPerspectiveFar(float farClip)
		{
			m_perspectiveFar = farClip;
			ReCalcProjection();
		}

	private:
		void ReCalcProjection();
	private:
		ProjectionType m_projectionType;
		float m_aspectRatio;
		float m_orthoGraphicSize;
		float m_orthoGraphicNear;
		float m_orthoGraphicFar;

		float m_perspectiveFOV;
		float m_perspectiveNear;
		float m_perspectiveFar;
	};
}