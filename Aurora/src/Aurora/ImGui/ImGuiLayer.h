#pragma once
#include "Aurora/Core.h"
#include "Aurora/Layer.h"
namespace Aurora
{
	class AURORA_API ImGuiLayer :public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnDetach()override;
		void OnAttach()override;
		void OnUpdate()override;
		void OnEvent(const Event& e)override;
	private:
		bool OnWindowsResizeEvent(const WindowResizeEvent& e);
		bool OnMouseMovedEvent(const MouseMovedEvent& e);
		bool OnMouseScrolledEvent(const MouseScrolledEvent& e);
		bool OnMouseButtonPressedEvent(const MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(const MouseButtonReleasedEvent&e);
		bool OnKeyPressedEvent(const KeyPressedEvent& e);
		bool OnKeyReleasedEvent(const KeyReleasedEvent& e);
		bool OnKeyTypedEvent(const KeyTypedEvent&e);
	private:
		float m_time;
	};
}