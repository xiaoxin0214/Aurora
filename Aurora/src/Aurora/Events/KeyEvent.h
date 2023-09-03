#pragma once
#include "Event.h"
namespace Aurora
{
	class AURORA_API KeyEvent :public Event
	{
	public:
		inline int GetKeyCode()const
		{
			return m_keyCode;
		}

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)
	protected:
		KeyEvent(int keyCode) :m_keyCode(keyCode)
		{

		}
	private:
		int m_keyCode;
	};

	class AURORA_API KeyPressedEvent :public KeyEvent {
	public:
		KeyPressedEvent(int keyCode, int repeatCount) :KeyEvent(keyCode), m_repeatCount(repeatCount)
		{

		}

		inline int GetRepeatCount()const
		{
			return m_repeatCount;
		}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent:" << GetKeyCode() << "(" << m_repeatCount << " repeats)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed)
	private:
		int m_repeatCount;
	};

	class AURORA_API KeyReleasedEvent :public KeyEvent {
	public:
		KeyReleasedEvent(int keyCode) :KeyEvent(keyCode)
		{
		}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent:" << GetKeyCode();
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class AURORA_API KeyTypedEvent :public KeyEvent {
	public:
		KeyTypedEvent(int keyCode) :KeyEvent(keyCode)
		{
		}

		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent:" << GetKeyCode();
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}