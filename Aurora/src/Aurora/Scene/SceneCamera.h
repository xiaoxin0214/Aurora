#pragma once
#include "Aurora/Core/Core.h"
#include "Aurora/Renderer/Camera.h"
namespace Aurora
{
	class AURORA_API SceneCamera :public Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera();
		void SetViewportSize(std::uint32_t width, std::uint32_t height);
		void SetOrthoGraphic(float size,float nearClip,float farClip);
		void SetOrthoGraphicSize(float size)
		{
			m_orthoGraphicSize = size;
			ReCalcProjection();
		}
		float GetOrthoGraphicSize()const
		{
			return m_orthoGraphicSize;
		}
	private:
		void ReCalcProjection();
	private:
		float m_aspectRatio;
		float m_orthoGraphicSize;
		float m_orthoGraphicNear;
		float m_orthoGraphicFar;
	};
}