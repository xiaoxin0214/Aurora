#pragma once
#include "Core.h"
#include "Aurora/Events/Event.h"
#include "Aurora/Events/ApplicationEvent.h"
#include "Aurora/Events/KeyEvent.h"
#include "Aurora/Events/MouseEvent.h"
#include "Aurora/Core/Timestep.h"
namespace Aurora
{
	class AURORA_API Layer
	{
	public:
		Layer(const std::string& name);
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(Timestep&timestep) {};
		virtual void OnEvent(const Event& e) {};
		virtual void OnImGuiRender() {};
		inline const std::string& GetLayerName()const {
			return m_debugName;
		}

	private:
		std::string      m_debugName;
	};
}