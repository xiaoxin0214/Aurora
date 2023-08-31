#pragma once
#include "pch.h"
#include "Core.h"
#include "Aurora/Events/Event.h"
namespace Aurora
{
	struct WindowProps {
		std::string title;
		std::uint32_t width;
		std::uint32_t height;

		WindowProps() :title("Aurora"), width(1960), height(1080)
		{
		}

		WindowProps(const std::string&name) :title(name), width(1960), height(1080)
		{
		}
	};

	class AURORA_API Window {
	public:
		using EventCallback = std::function<void(Event&)>;
	public:
		virtual ~Window() {};
		virtual void OnUpdate() = 0;
		virtual unsigned int GetWidth()const = 0;
		virtual unsigned int GetHeight()const = 0;
		virtual void SetEventCallback(const EventCallback& callback) = 0;

		virtual void SetVSync(bool enabled) = 0;
		virtual bool GetVSync()const = 0;

		virtual void* GetNativeWindow()const=0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}