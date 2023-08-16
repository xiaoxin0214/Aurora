#pragma once
#include "Aurora/Core/Window.h"
struct GLFWwindow;
namespace Aurora
{
	class GraphicsContext;
	class AURORA_API WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate()override;

		inline unsigned int GetWidth()const override {
			return m_data.width;
		}

		inline unsigned int GetHeight()const override {
			return m_data.height;
		}

		inline void SetEventCallback(const EventCallback& callback) override {
			m_data.callback = callback;
		}

		void SetVSync(bool enabled)override;
		inline bool GetVSync()const override
		{
			return m_data.vSync;
		}

		void* GetNativeWindow()const override {
			return m_pWindow;
		}
	private:
		void Init(const WindowProps& props);
		void ShutDown();
	private:
		struct WindowData {
			std::string title;
			unsigned int width;
			unsigned int height;
			bool         vSync;
			EventCallback callback;
		};

		WindowData          m_data;
		GLFWwindow* m_pWindow;
		GraphicsContext* m_pContext;
	};
}