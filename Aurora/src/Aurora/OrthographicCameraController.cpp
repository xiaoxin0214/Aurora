#include "pch.h"
#include "OrthographicCameraController.h"
#include "Aurora/Input.h"
#include "Aurora/KeyCodes.h"

namespace Aurora
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio) :m_zoomLevel(1.0f), m_aspectRatio(aspectRatio), m_cameraMoveSpeed(1.0f), m_cameraPosition(glm::vec3(0.0f)), m_cameraRotation(0.0f), m_cameraRotationSpeed(180.0f), m_camera(-aspectRatio * m_zoomLevel, aspectRatio* m_zoomLevel, -m_zoomLevel, m_zoomLevel)
	{

	}

	OrthographicCameraController::~OrthographicCameraController()
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		float ts = timestep;
		// 在updata中做而不是在事件中做，在事件中做移动不够丝滑
		if (Input::IsKeyPressed(AURORA_KEY_A))
		{
			m_cameraPosition.x += m_cameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed(AURORA_KEY_W))
		{
			m_cameraPosition.y -= m_cameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed(AURORA_KEY_S))
		{
			m_cameraPosition.y += m_cameraMoveSpeed * ts;
		}
		else if (Input::IsKeyPressed(AURORA_KEY_D))
		{
			m_cameraPosition.x -= m_cameraMoveSpeed * ts;
		}

		if (Input::IsKeyPressed(AURORA_KEY_Q))
		{
			m_cameraRotation -= m_cameraRotationSpeed * ts;
		}
		else if (Input::IsKeyPressed(AURORA_KEY_E))
		{
			m_cameraRotation += m_cameraRotationSpeed * ts;
		}

		m_camera.SetPosition(m_cameraPosition);
		m_camera.SetRotation(m_cameraRotation);
	}

	void OrthographicCameraController::OnEvent(const Event& e)
	{
		EventDispatcher dispatcher(const_cast<Event&>(e));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
		dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
	}

	bool OrthographicCameraController::OnWindowResize(const WindowResizeEvent& e)
	{
		m_aspectRatio = (e.GetWidth()*1.0f) / e.GetHeight();
		m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnMouseScrolled(const MouseScrolledEvent& e)
	{
		m_zoomLevel-=e.GetOffsetY()*0.25f;
		m_zoomLevel = std::max(m_zoomLevel,0.25f);
		m_camera.SetProjection(-m_aspectRatio*m_zoomLevel,m_aspectRatio*m_zoomLevel,-m_zoomLevel,m_zoomLevel);
		return false;
	}
}