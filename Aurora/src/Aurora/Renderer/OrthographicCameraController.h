#pragma once
#include "Aurora/Core/Core.h"
#include "Aurora/Renderer/OrthographicCamera.h"
#include "Aurora/Core/Timestep.h"
#include "Aurora/Events/Event.h"
#include "Aurora/Events/ApplicationEvent.h"
#include "Aurora/Events/MouseEvent.h"
namespace Aurora
{
	class AURORA_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio);
		~OrthographicCameraController();
	public:
		void OnUpdate(Timestep timestep);
		void OnEvent(const Event& e);

		const OrthographicCamera& GetCamera()const
		{
			return m_camera;
		}
	private:
		bool OnWindowResize(const WindowResizeEvent&e);
		bool OnMouseScrolled(const MouseScrolledEvent& e);
	private:
		float              m_zoomLevel;
		float              m_aspectRatio;
		float              m_cameraMoveSpeed;
		float              m_cameraRotation;
		float              m_cameraRotationSpeed;
		OrthographicCamera m_camera;
		glm::vec3          m_cameraPosition;
	};
}