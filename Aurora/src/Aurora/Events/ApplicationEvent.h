#pragma once
#include "Event.h"

namespace Aurora
{

	class AURORA_API WindowResizeEvent :public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height) :m_width(width), m_height(height)
		{
		}

		inline unsigned int GetWidth()const { return m_width; }
		inline unsigned int GetHeight()const { return m_height; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent:" << m_width << m_height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_width;
		unsigned int m_height;
	};

	class AURORA_API WindowCloseEvent :public Event
	{
	public:
		WindowCloseEvent()
		{
		}

		EVENT_CLASS_TYPE(WindowClose)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AURORA_API ApplicationTickEvent :public Event
	{
	public:
		ApplicationTickEvent()
		{
		}

		EVENT_CLASS_TYPE(AppTick)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AURORA_API ApplicationUpdateEvent :public Event
	{
	public:
		ApplicationUpdateEvent()
		{
		}

		EVENT_CLASS_TYPE(AppUpdate)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AURORA_API ApplicationRenderEvent :public Event
	{
	public:
		ApplicationRenderEvent()
		{
		}

		EVENT_CLASS_TYPE(AppRender)
			EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

}