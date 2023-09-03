#pragma once

#include "Aurora/Core/Core.h"
namespace Aurora
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){ return EventType::##type; }\
								virtual EventType GetEventType()const override  { return GetStaticType(); }\
								virtual const char* GetName()const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags()const override   { return category; } 

	class AURORA_API Event
	{
		friend class EventDispatcher;
	public:
		Event() :IsHandled(false) {}
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags()const = 0;
		virtual std::string ToString()const { return GetName(); }
		inline bool IsInCategory(EventCategory category)const {
			return GetCategoryFlags() & category;
		}

	public:
		bool IsHandled;
	};


	class AURORA_API EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) :m_event(event)
		{
		}

		template<typename T>
		bool dispatch(EventFn<T> func)
		{
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.IsHandled = func(*(T*)(&m_event));
				return true;
			}
			return false;
		}

	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& event)
	{
		return os << event.ToString();
	}
}