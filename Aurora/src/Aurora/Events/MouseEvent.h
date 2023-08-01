#pragma once
#include "Event.h"

namespace Aurora
{
	class AURORA_API MouseMovedEvent :public Event
	{
	public:
		MouseMovedEvent(float x, float y) :m_mouseX(x), m_mouseY(y)
		{

		}

		inline float GetX()const { return m_mouseX; }
		inline float GetY()const { return m_mouseY; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent:" << m_mouseX << "," << m_mouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_mouseX;
		float m_mouseY;
	};

	class AURORA_API MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(float offsetX, float offsetY) :m_offsetX(offsetX), m_offsetY(offsetY)
		{
		}

		inline float GetOffsetX()const { return m_offsetX; }
		inline float GetOffsetY()const { return m_offsetY; }

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent:" << m_offsetX << "," << m_offsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled)
			EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_offsetX;
		float m_offsetY;
	};

	class AURORA_API MouseButtonEvent :public Event
	{
	public:
		inline int GetButton()const { return m_button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button) :m_button(button)
		{

		}
	private:
		int m_button;
	};

	class AURORA_API MouseButtonPressedEvent :public MouseButtonEvent {
	public:
		MouseButtonPressedEvent(int button) :MouseButtonEvent(button)
		{

		}

		EVENT_CLASS_TYPE(MouseButtonPressed)

			std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent:" << GetButton();
			return ss.str();
		}
	};

	class AURORA_API MouseButtonReleasedEvent :public MouseButtonEvent {
	public:
		MouseButtonReleasedEvent(int button) :MouseButtonEvent(button)
		{

		}

		EVENT_CLASS_TYPE(MouseButtonReleased)

			std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent:" << GetButton();
			return ss.str();
		}
	};
}